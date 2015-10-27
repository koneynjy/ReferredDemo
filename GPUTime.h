#include "d3dUtil.h"

class GPUTime
{
public :
	GPUTime(ID3D11Device* device, ID3D11DeviceContext* context);
private :
	ID3D11Query *timeQuery;

};