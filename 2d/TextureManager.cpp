#include"TextureManager.h"

TextureManager* TextureManager::GetInstance()
{
	if (instance == NULL)
	{
		instance = new TextureManager;
	}
	return instance;
}

TextureManager::~TextureManager()
{

}

void TextureManager::Initialize( int32_t width, int32_t height)
{
	dxCommon_ = DirectXCommon::GetInstance();
	engine_ = MyEngine::GetInstance();

	descriptorSizeSRV = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

}

void TextureManager::Update()
{

}

uint32_t TextureManager::LoadTexture(const std::string& filePath)
{
	uint32_t index = TextureCount + 1;

	
	for (int i = 0; i < kMaxTexture; i++)
	{
		if (textures_[i].filename == filePath)
		{
			return textures_[i].textureHandle;
		}
	}

	for (int i = 0; i < kMaxTexture; ++i) {
		if (IsusedTexture[i] == false) {
			index = i;
			IsusedTexture[i] = true;
			break;
		}
	}
	if (index < 0) {
		//0より少ない
		assert(false);
	}
	if (kMaxTexture < index) {
		//MaxSpriteより多い
		assert(false);
	}

	//Textureを読んで転送する
	DirectX::ScratchImage mipImages = ImageFileOpen(filePath);
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	textures_.at(index).textureResource = CreateTextureResource(dxCommon_->GetDevice(), metadata);
	intermediateResource.at(index) = UploadTextureData(textures_.at(index).textureResource.Get(), mipImages);
	//metadataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);
	//SRVを作成するDescriptorHeapの場所を決める
	textures_.at(index).textureSrvHandleCPU = GetCPUDescriptorHandle(dxCommon_->GetsrvDescriptorHeap(), descriptorSizeSRV, index);
	textures_.at(index).textureSrvHandleGPU = GetGPUDescriptorHandle(dxCommon_->GetsrvDescriptorHeap(), descriptorSizeSRV, index);
	//先頭はImGuiが使っているので次のを使う
	textures_.at(index).textureSrvHandleCPU.ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textures_.at(index).textureSrvHandleGPU.ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textures_.at(index).filename = filePath;
	textures_.at(index).textureHandle = index;

	//SRVの作成
	dxCommon_->GetDevice()->CreateShaderResourceView(textures_.at(index).textureResource.Get(), &srvDesc, textures_.at(index).textureSrvHandleCPU);

	return index;
}

DirectX::ScratchImage TextureManager::ImageFileOpen(const std::string& filePath)
{
	//テクスチャファイルを読み込みプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));
	//ミップマップの作成
	DirectX::ScratchImage mipImage{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImage);

	return mipImage;
}

Microsoft::WRL::ComPtr< ID3D12Resource> TextureManager::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata)
{
	//metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);
	resourceDesc.Height = UINT(metadata.height);
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);
	resourceDesc.Format = metadata.format;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);
	//利用するheapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;//細かい設定を行う

	//Resourceの生成
	Microsoft::WRL::ComPtr< ID3D12Resource> resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,//Heapの設定
		D3D12_HEAP_FLAG_NONE,//Heapの特殊な設定
		&resourceDesc,//Resourceの設定
		D3D12_RESOURCE_STATE_COPY_DEST,//初回のResourceState　Textureは基本読むだけ
		nullptr,//Clear最適値　使わないためnullptr
		IID_PPV_ARGS(&resource)//作成するResourceポインタへのポインタ
	);
	return resource;
}

[[nodiscard]]
Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages)
{
	std::vector<D3D12_SUBRESOURCE_DATA>subresources;
	DirectX::PrepareUpload(dxCommon_->GetDevice(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture, 0, UINT(subresources.size()));
	Microsoft::WRL::ComPtr< ID3D12Resource> intermediateResource = engine_->CreateBufferResource(intermediateSize);
	UpdateSubresources(dxCommon_->GetCommandList(), texture, intermediateResource.Get(), 0, 0, UINT(subresources.size()), subresources.data());
	//Tetureへの転送後は利用できるようにD3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier);
	return intermediateResource;
}

D3D12_CPU_DESCRIPTOR_HANDLE TextureManager::GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}
D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

TextureManager* TextureManager::instance = NULL;
