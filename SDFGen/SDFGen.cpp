#include "SDF.h"
#include <string>
#include <iostream>
#include <vector>
#include "io.h"
//#pragma optimize("", off)
using std::string;
using std::wstring;
using std::vector;

bool InitDirect3D(ID3D11Device* &md3dDevice, ID3D11DeviceContext* &md3dImmediateContext)
{
	// Create the device and device context.
	D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
		0,                 // default adapter
		md3dDriverType,
		0,                 // no software device
		createDeviceFlags,
		0, 0,              // default feature level array
		D3D11_SDK_VERSION,
		&md3dDevice,
		&featureLevel,
		&md3dImmediateContext);

	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}
	return true;
}

int main()
{
	string file_name = "D:/lod/proxy/";
	string path = file_name + "*.*";
	_finddata_t file;
	int cnt = 0;
	long lf;
	if ((lf = _findfirst(path.c_str(), &file)) == -1l)//_findfirst返回的是long型; long __cdecl _findfirst(const char *, struct _finddata_t *)
		return 0;
	else
	{
		while (_findnext(lf, &file) == 0)//int __cdecl _findnext(long, struct _finddata_t *);如果找到下个文件的名字成功的话就返回0,否则返回-1
		{
			if (file.attrib != _A_SUBDIR)
			{
				string name(file.name);
				if (name.length() > 6 &&
					name.substr(name.length() - 6, 6) == ".model")  {
					std::cout <<cnt++ << " " << file_name + name << std::endl;
					continue;
					CMesh cmesh;
					short x = 0, z = 0;
					sscanf(name.substr(0, 4).c_str(), "%hx", &x);
					sscanf(name.substr(4, 4).c_str(), "%hx", &z);
					cmesh.Init(file_name + name, "D:/", XMFLOAT3(x * 100 + 50.0, 0, z * 100 + 50.0));

					ID3D11Device* md3dDevice;
					ID3D11DeviceContext* md3dImmediateContext;
					InitDirect3D(md3dDevice, md3dImmediateContext);
					//cmesh.Init("D:/combineObjPath/combineObjPath/0038fff2o/0038fff2o.obj", "D:/", XMFLOAT3(0, 0, 0));
					//cmesh.Init("D:/lod/proxy/000e000co.model", "D:/", XMFLOAT3(0, 0, 0));
					SDFModel sdf(cmesh);
					sdf.GenerateSDF(10.0f, false);
					float* testData = NULL;
					unsigned w, h, d;
					sdf.GetSDFData(testData, w, h, d);
					D3D11_TEXTURE3D_DESC texDesc;
					texDesc.Width = w;
					texDesc.Height = h;
					texDesc.Depth = d;
					texDesc.MipLevels = 1;
					texDesc.Format = DXGI_FORMAT_R32_FLOAT;
					texDesc.Usage = D3D11_USAGE_DEFAULT;
					texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
					texDesc.CPUAccessFlags = 0;
					texDesc.MiscFlags = 0;

					D3D11_SUBRESOURCE_DATA subdata;
					subdata.pSysMem = testData;
					subdata.SysMemPitch = w * sizeof(float);
					subdata.SysMemSlicePitch = w * d * sizeof(float);
					ID3D11Texture3D* SDFMap = 0;
					string dds = file_name + name.substr(0, 9) + ".dds";
					wstring wdds(dds.length(), L' ');
					std::copy(dds.begin(), dds.end(), wdds.begin());
					HR(md3dDevice->CreateTexture3D(&texDesc, &subdata, &SDFMap));
					HR(D3DX11SaveTextureToFile(
						md3dImmediateContext,
						SDFMap,
						D3DX11_IFF_DDS,
						wdds.c_str()
						));

					std::cout << dds << std::endl;
				}
			}
			else
			{
				std::cout << file.attrib << std::endl;
				std::cout << file.name << std::endl;
			}
		}
	}
	_findclose(lf);
}
