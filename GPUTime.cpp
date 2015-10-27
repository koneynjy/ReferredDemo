#include "GPUTime.h"

GPUTime::GPUTime(ID3D11Device* device, ID3D11DeviceContext* context)
{
	D3D11_QUERY_DESC desc;
	desc.Query = D3D11_QUERY_TIMESTAMP;
	desc.MiscFlags = 0;

	HR(device->CreateQuery(&desc, &timeQuery));
}