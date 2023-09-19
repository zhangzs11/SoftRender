#include"Graphics.h"
void Texture2D::Release()
{}
void RenderTargetView::Release()
{

}
void DepthStencilView::Release()
{

}
void VIEWPORT::Release()
{

}
HRESULT Device::CreateRenderTargetView(Texture2D* pTexture, RenderTargetView** ppRTView)//创建渲染目标视图
{
	*ppRTView = new RenderTargetView();
	(*ppRTView)->texture2d = pTexture;
	return S_OK;
}
HRESULT Device::CreateDepthstencView(Texture2D* pTexture, DepthStencilView** ppDSView)
{
	*ppDSView = new DepthStencilView();
	pTexture = new Texture2D();
	for (int i = 0; i < 1024; i++) {
		for (int j = 0; j < 768; j++) {
			*(*(pTexture->data + i) + j) = COLOR(INT_MAX, 0, 0, 0);
		}
	}
	(*ppDSView)->texture2d = pTexture;
	return S_OK;
}
HRESULT Device::CreateBuffer(const BUFFER_DESC* pDesc, SUBRESOURCE_DATA* pInitialData, Buffer** ppBuffer)
{
	*ppBuffer = new Buffer();
	(*ppBuffer)->desc = *pDesc;
	if (pInitialData != NULL) {
		(*ppBuffer)->data = *pInitialData;
	}
	return S_OK;
}
HRESULT Device::CreateViewPort(VIEWPORT** vp, int* width, int* height)
{
	*vp = new VIEWPORT();
	(*vp)->Width = *width;
	(*vp)->Height = *height;
	return S_OK;
}
HRESULT Device::CreateMesh(Mesh** pmesh)
{
	*pmesh = new Mesh();
	return S_OK;
}
void Device::Release()
{

}
void DeviceContext::SetRenderTarget(UINT NumViews, RenderTargetView* ppRenderTargetViews, DepthStencilView* pDepthStencilView)//设置要渲染到的那个目标视图
{
	this->rendertargetview = ppRenderTargetViews;
	this->depthstencilview = pDepthStencilView;
}
void DeviceContext::SetViewports(UINT numviewports, VIEWPORT* pViewports)//设置视窗范围
{
	this->viewport = pViewports;
}
void DeviceContext::SetVertexBuffer(UINT Numbuffers, Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets)//设置顶点缓冲
{
	this->vertexbuffer = *ppVertexBuffers;
}
void DeviceContext::SetIndexBuffer(Buffer* pIndexBuffer, UINT Offset)
{
	this->indexbuffer = pIndexBuffer;
}
void DeviceContext::SetConstantBuffer(UINT StartSlot, UINT NumBuffers, Buffer* const* ppConstantBuffers)
{
	this->constantbuffer = *ppConstantBuffers;
}
void DeviceContext::SetPrimitiveTopology(UINT primitive)//设置图元类型
{
	this->primative = primitive;
}
void DeviceContext::SetMesh(Mesh* pMesh)
{
	this->meshsource = pMesh;
}

void DeviceContext::Draw()
{

}
void DeviceContext::DarwIndexed(UINT IndexCount, UINT startIndexLocation, UINT BaseVertexLocation)
{
	UINT* pindexdata = (UINT*)(this->indexbuffer->data.pSyMem);//当前循环的索引
	SimpleVertex* pvertexdata = (SimpleVertex*)(this->vertexbuffer->data.pSyMem);
	UINT index;
	UINT index_2;
	UINT index_1;
	SimpleVertex vertex;
	SimpleVertex vertex_2;
	SimpleVertex vertex_1;
	VECTOR4 point;
	std::vector<Triangle> triangleVector;
	triangleVector.clear();
	for (int i = 0; i < IndexCount; i++) {//遍历索引，获得一个经过透视变换的三角形列表
		if ((i + 1) % 3 == 0) {
			index_2 = *(pindexdata + i - 2);
			vertex_2 = *(pvertexdata + index_2);
			vertex_2 = this->VertexShader(vertex_2);

			index_1 = *(pindexdata + i - 1);
			vertex_1 = *(pvertexdata + index_1);
			vertex_1 = this->VertexShader(vertex_1);

			index = *(pindexdata + i);
			vertex = *(pvertexdata + index);
			vertex = this->VertexShader(vertex);

			Triangle triangle;
			triangle.vertex[0] = vertex_2;
			triangle.vertex[1] = vertex_1;
			triangle.vertex[2] = vertex;
			triangleVector.push_back(triangle);
		}
	}
	//裁剪
	float Near = 0.01;
	float Far = 50;
	triangleVector = SutherlandHodgeman(triangleVector, Near, Far);
	int trianglesize = triangleVector.size();
	//透视除法，画三角形和线框
	for (int i = 0; i < trianglesize; i++) {
		for (int j = 0; j < 3; j++) {
			triangleVector[i].vertex[j] = wviewport(triangleVector[i].vertex[j]);
		}
		if (!FaceCull(triangleVector[i].vertex[0].Pos, triangleVector[i].vertex[1].Pos, triangleVector[i].vertex[2].Pos)) {//背面剔除
			drawTriangle(triangleVector[i].vertex[0], triangleVector[i].vertex[1], triangleVector[i].vertex[2]);
		}
		DDA_Line(triangleVector[i].vertex[0].Pos.x, triangleVector[i].vertex[1].Pos.x, triangleVector[i].vertex[0].Pos.y, triangleVector[i].vertex[1].Pos.y, COLOR(0, 255, 0, 1), COLOR(0, 255, 0, 1), this);
		DDA_Line(triangleVector[i].vertex[0].Pos.x, triangleVector[i].vertex[2].Pos.x, triangleVector[i].vertex[0].Pos.y, triangleVector[i].vertex[2].Pos.y, COLOR(0, 255, 0, 1), COLOR(0, 255, 0, 1), this);
		DDA_Line(triangleVector[i].vertex[1].Pos.x, triangleVector[i].vertex[2].Pos.x, triangleVector[i].vertex[1].Pos.y, triangleVector[i].vertex[2].Pos.y, COLOR(0, 255, 0, 1), COLOR(0, 255, 0, 1), this);
	}
}
void DeviceContext::DrawMesh()
{
	VECTOR4 temp;
	Mesh* meshdata = this->meshsource;//模型文件所有信息
	std::vector<Triangle> triangleVector;//三角形列表
	
	int indexBuffersize = meshdata->indexBuffer.size();
	for (int i = 0; i < indexBuffersize; i++) {//循环每一行，也就是每一个面
		for (int m = 0; m < meshdata->indexBuffer[i].size() - 2; m++) {//循环每一个三角形
			SimpleVertex v1;
			SimpleVertex v2;
			SimpleVertex v3;

			v1.Pos = meshdata->positionBuffer[meshdata->indexBuffer[i][0].x - 1];
			v1.uv = meshdata->uvBuffer[meshdata->indexBuffer[i][0].y - 1];
			v1.Normal = meshdata->normalBuffer[meshdata->indexBuffer[i][0].z - 1];
			v1.Color = COLOR(0, 255, 0, 1);

			v2.Pos = meshdata->positionBuffer[meshdata->indexBuffer[i][m+1].x - 1];
			v2.uv = meshdata->uvBuffer[meshdata->indexBuffer[i][m+1].y - 1];
			v2.Normal = meshdata->normalBuffer[meshdata->indexBuffer[i][m+1].z - 1];
			v2.Color = COLOR(0, 255, 0, 1);

			v3.Pos = meshdata->positionBuffer[meshdata->indexBuffer[i][m+2].x - 1];
			v3.uv = meshdata->uvBuffer[meshdata->indexBuffer[i][m+2].y - 1];
			v3.Normal = meshdata->normalBuffer[meshdata->indexBuffer[i][m+2].z - 1];
			v3.Color = COLOR(0, 255, 0, 1);

			VECTOR4 tangent;
			VECTOR4 bitangent;
			VECTOR4 edge1;
			VECTOR4 edge2;
			VECTOR4 deltaUV1;
			VECTOR4 deltaUV2;
			float f;
			//求v1的切线和副切线
			edge1 = v3.Pos - v1.Pos;
			edge2 = v2.Pos - v1.Pos;
			temp = v3.uv - v1.uv;
			deltaUV1 = Vec4Normalize(&temp);
			temp = v2.uv - v1.uv;
			deltaUV2 = Vec4Normalize(&temp);
			f = 1 / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
			tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
			tangent = Vec4Normalize(&tangent);

			bitangent.x = f * (( - deltaUV2.x) * edge1.x + deltaUV1.x * edge2.x);
			bitangent.y = f * (( - deltaUV2.x) * edge1.y + deltaUV1.x * edge2.y);
			bitangent.z = f * (( - deltaUV2.x) * edge1.z + deltaUV1.x * edge2.z);
			bitangent = Vec4Normalize(&bitangent);

			temp = tangent - v1.Normal * Vec4Dot(&tangent, &v1.Normal);
			tangent = Vec4Normalize(&temp);
			temp = bitangent - v1.Normal * Vec4Dot(&bitangent, &v1.Normal) - tangent * Vec4Dot(&bitangent, &tangent);
			bitangent = Vec4Normalize(&temp);
			v1.tangent = tangent;
			v1.bitangent = bitangent;
			//求v2的切线和副切线
			edge1 = v1.Pos - v2.Pos;
			edge2 = v3.Pos - v2.Pos;
			temp = v1.uv - v2.uv;
			deltaUV1 = Vec4Normalize(&temp);
			temp = v3.uv - v2.uv;
			deltaUV2 = Vec4Normalize(&temp);
			f = 1 / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
			tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
			tangent = Vec4Normalize(&tangent);

			bitangent.x = f * ((-deltaUV2.x) * edge1.x + deltaUV1.x * edge2.x);
			bitangent.y = f * ((-deltaUV2.x) * edge1.y + deltaUV1.x * edge2.y);
			bitangent.z = f * ((-deltaUV2.x) * edge1.z + deltaUV1.x * edge2.z);
			bitangent = Vec4Normalize(&bitangent);

			temp = tangent - v2.Normal * Vec4Dot(&tangent, &v2.Normal);
			tangent = Vec4Normalize(&temp);
			temp = bitangent - v2.Normal * Vec4Dot(&bitangent, &v2.Normal) - tangent * Vec4Dot(&bitangent, &tangent);
			bitangent = Vec4Normalize(&temp);
			v2.tangent = tangent;
			v2.bitangent = bitangent;
			//求v3的切线和副切线
			edge1 = v2.Pos - v3.Pos;
			edge2 = v1.Pos - v3.Pos;
			temp = v2.uv - v3.uv;
			deltaUV1 = Vec4Normalize(&temp);
			temp = v1.uv - v3.uv;
			deltaUV2 = Vec4Normalize(&temp);
			f = 1 / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
			tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
			tangent = Vec4Normalize(&tangent);

			bitangent.x = f * ((-deltaUV2.x) * edge1.x + deltaUV1.x * edge2.x);
			bitangent.y = f * ((-deltaUV2.x) * edge1.y + deltaUV1.x * edge2.y);
			bitangent.z = f * ((-deltaUV2.x) * edge1.z + deltaUV1.x * edge2.z);
			bitangent = Vec4Normalize(&bitangent);

			temp = tangent - v3.Normal * Vec4Dot(&tangent, &v3.Normal);
			tangent = Vec4Normalize(&temp);
			temp = bitangent - v3.Normal * Vec4Dot(&bitangent, &v3.Normal) - tangent * Vec4Dot(&bitangent, &tangent);
			bitangent = Vec4Normalize(&temp);
			v3.tangent = tangent;
			v3.bitangent = bitangent;

			v1 = this->VertexShader(v1);
			v2 = this->VertexShader(v2);
			v3 = this->VertexShader(v3);

			Triangle triangle;
			triangle.vertex[0] = v1;
			triangle.vertex[1] = v2;
			triangle.vertex[2] = v3;
			triangleVector.push_back(triangle);
		}
	}
	//裁剪
	float Near = 0.01;//近平面
	float Far = 50;//远平面
	triangleVector = SutherlandHodgeman(triangleVector, Near, Far);
	int trianglesize = triangleVector.size();

	//透视除法，画三角形和线框
	for (int i = 0; i < trianglesize; i++) {
		for (int j = 0; j < 3; j++) {
			triangleVector[i].vertex[j] = wviewport(triangleVector[i].vertex[j]);
		}
		if (!FaceCull(triangleVector[i].vertex[0].Pos, triangleVector[i].vertex[1].Pos, triangleVector[i].vertex[2].Pos)) {//背面剔除
			if (this->rendermodel == 2 || this->rendermodel == 3) {
				drawTriangle(triangleVector[i].vertex[0], triangleVector[i].vertex[1], triangleVector[i].vertex[2]);
			}
		}
		if (this->rendermodel == 1 || this->rendermodel == 3) {
			DDA_Line(triangleVector[i].vertex[0].Pos.x, triangleVector[i].vertex[1].Pos.x, triangleVector[i].vertex[0].Pos.y, triangleVector[i].vertex[1].Pos.y, COLOR(0, 255, 0, 1), COLOR(0, 255, 0, 1), this);
			DDA_Line(triangleVector[i].vertex[0].Pos.x, triangleVector[i].vertex[2].Pos.x, triangleVector[i].vertex[0].Pos.y, triangleVector[i].vertex[2].Pos.y, COLOR(0, 255, 0, 1), COLOR(0, 255, 0, 1), this);
			DDA_Line(triangleVector[i].vertex[1].Pos.x, triangleVector[i].vertex[2].Pos.x, triangleVector[i].vertex[1].Pos.y, triangleVector[i].vertex[2].Pos.y, COLOR(0, 255, 0, 1), COLOR(0, 255, 0, 1), this);
		}
	}
}
void DeviceContext::DrawLight()
{
	ConstantBuffer* cb = (ConstantBuffer*)this->constantbuffer->data.pSyMem;
	SimpleVertex lightvertex;
	lightvertex.Pos = cb->lightPos;
	lightvertex = this->VertexShader(lightvertex);
	lightvertex = wviewport(lightvertex);
	for (int i = 0; i < 10; i++) {
		for (int m = 0; m < 10; m++) {
			if (m + int(lightvertex.Pos.x) >= 0 && m + int(lightvertex.Pos.x) < 1024 && i + int(lightvertex.Pos.y) >= 0 && i + int(lightvertex.Pos.y) < 768) {
				this->rendertargetview->texture2d->data[m + int(lightvertex.Pos.x)][i + int(lightvertex.Pos.y)] = cb->id;
			}
		}
	}
}
void DeviceContext::fillBottomFlatTriangle(SimpleVertex v1, SimpleVertex v2, SimpleVertex v3)
{
	double invslope1 = (v2.Pos.x - v1.Pos.x) / (v2.Pos.y - v1.Pos.y);
	double invslope2 = (v3.Pos.x - v1.Pos.x) / (v3.Pos.y - v1.Pos.y);
	float curx1 = v1.Pos.x;
	float curx2 = v1.Pos.x;

	if (invslope1 > invslope2) {//invslope1,v2代表左边的边
		float temp;
		temp = invslope1;
		invslope1 = invslope2;
		invslope2 = temp;
		SimpleVertex temp1;
		temp1 = v2;
		v2 = v3;
		v3 = temp1;
	}
	VECTOR4 dx = VECTOR4((-v2.Pos.y + v3.Pos.y) / 2, (-v3.Pos.y + v1.Pos.y) / 2, (-v1.Pos.y + v2.Pos.y) / 2, 1) / CalTriangleArea(v1.Pos, v2.Pos, v3.Pos);
	VECTOR4 dy = VECTOR4((-v3.Pos.x + v2.Pos.x) / 2, (-v1.Pos.x + v3.Pos.x) / 2, (-v2.Pos.x + v1.Pos.x) / 2, 1) / CalTriangleArea(v1.Pos, v2.Pos, v3.Pos);
	float dxdenominator = dx.x * v1.Pos.w + dx.y * v2.Pos.w + dx.z * v3.Pos.w;
	float dydenominator = dy.x * v1.Pos.w + dy.y * v2.Pos.w + dy.z * v3.Pos.w;

	//初始uv
	VECTOR4 uv = v1.uv;
	VECTOR4 uvnumerator = v1.uv * v1.Pos.w;
	float uvdenominator = v1.Pos.w;
	//考虑透视矫正算uv增量
	VECTOR4 uvdxnnumerator = v1.uv * v1.Pos.w * dx.x + v2.uv * v2.Pos.w * dx.y + v3.uv * v3.Pos.w * dx.z;//uv在x增量
	VECTOR4 uvdynnumerator = v1.uv * v1.Pos.w * dy.x + v2.uv * v2.Pos.w * dy.y + v3.uv * v3.Pos.w * dy.z;//uv在y增量

	//初始normal
	VECTOR4 Normal = v1.Normal;
	VECTOR4 Normalnumerator = v1.Normal * v1.Pos.w;
	float Normaldenominator = v1.Pos.w;
	//考虑透视矫正算normal增量
	VECTOR4 Normaldxnnumerator = v1.Normal * v1.Pos.w * dx.x + v2.Normal * v2.Pos.w * dx.y + v3.Normal * v3.Pos.w * dx.z;//normal在x增量
	VECTOR4 Normaldynnumerator = v1.Normal * v1.Pos.w * dy.x + v2.Normal * v2.Pos.w * dy.y + v3.Normal * v3.Pos.w * dy.z;//normal在y增量

	//初始世界坐标
	VECTOR4 Pos = v1.PosWorld;
	VECTOR4 Posnumerator = v1.PosWorld * v1.Pos.w;
	float Posdenominator = v1.Pos.w;
	//考虑透视矫正算世界坐标增量
	VECTOR4 Posdxnnumerator = v1.PosWorld * v1.Pos.w * dx.x + v2.PosWorld * v2.Pos.w * dx.y + v3.PosWorld * v3.Pos.w * dx.z;//世界坐标在x增量
	VECTOR4 Posdynnumerator = v1.PosWorld * v1.Pos.w * dy.x + v2.PosWorld * v2.Pos.w * dy.y + v3.PosWorld * v3.Pos.w * dy.z;//世界坐标在y增量

	//初始TBN矩阵
	MATRIX TBN = v1.TBN;
	MATRIX TBNnumerator = v1.TBN * v1.Pos.w;
	float TBNdenominator = v1.Pos.w;
	//考虑透视矫正算TBN增量
	MATRIX TBNdxnnumerator = v1.TBN * v1.Pos.w * dx.x + v2.TBN * v2.Pos.w * dx.y + v3.TBN * v3.Pos.w * dx.z;//TBN在x增量
	MATRIX TBNdynnumerator = v1.TBN * v1.Pos.w * dy.x + v2.TBN * v2.Pos.w * dy.y + v3.TBN * v3.Pos.w * dy.z;//TBN在y增量

	float ymax = min(v2.Pos.y, v3.Pos.y);
	for (int scanlineY = floor(v1.Pos.y); scanlineY < floor(ymax); scanlineY++) {
		uvnumerator = v1.uv * v1.Pos.w + uvdynnumerator * (scanlineY - v1.Pos.y) + uvdxnnumerator * (scanlineY - v1.Pos.y) * invslope1;//uv
		uvdenominator = v1.Pos.w + dydenominator * (scanlineY - v1.Pos.y) + dxdenominator * (scanlineY - v1.Pos.y) * invslope1;
		uv = uvnumerator / uvdenominator;

		Normalnumerator = v1.Normal * v1.Pos.w + Normaldynnumerator * (scanlineY - v1.Pos.y) + Normaldxnnumerator * (scanlineY - v1.Pos.y) * invslope1;//normal
		Normaldenominator = v1.Pos.w + dydenominator * (scanlineY - v1.Pos.y) + dxdenominator * (scanlineY - v1.Pos.y) * invslope1;
		Normal = Normalnumerator / Normaldenominator;

		Posnumerator = v1.PosWorld * v1.Pos.w + Posdynnumerator * (scanlineY - v1.Pos.y) + Posdxnnumerator * (scanlineY - v1.Pos.y) * invslope1;//pos
		Posdenominator = v1.Pos.w + dydenominator * (scanlineY - v1.Pos.y) + dxdenominator * (scanlineY - v1.Pos.y) * invslope1;
		Pos = Posnumerator / Posdenominator;

		TBNnumerator = v1.TBN * v1.Pos.w + TBNdynnumerator * (scanlineY - v1.Pos.y) + TBNdxnnumerator * (scanlineY - v1.Pos.y) * invslope1;//TBN
		TBNdenominator = v1.Pos.w + dydenominator * (scanlineY - v1.Pos.y) + dxdenominator * (scanlineY - v1.Pos.y) * invslope1;
		TBN = TBNnumerator / TBNdenominator;

		for (int x = floor(curx1); x < floor(curx2); x++) {
			uvnumerator = uvnumerator + uvdxnnumerator;
			uvdenominator = uvdenominator + dxdenominator;
			uv = uvnumerator / uvdenominator;

			Normalnumerator = Normalnumerator + Normaldxnnumerator;
			Normaldenominator = Normaldenominator + dxdenominator;
			Normal = Normalnumerator / Normaldenominator;

			Posnumerator = Posnumerator + Posdxnnumerator;
			Posdenominator = Posdenominator + dxdenominator;
			Pos = Posnumerator / Posdenominator;

			TBNnumerator = TBNnumerator + TBNdxnnumerator;
			TBNdenominator = TBNdenominator + dxdenominator;
			TBN = TBNnumerator / TBNdenominator;
			if (x > 0 && x < 1024 && scanlineY>0 && scanlineY < 768) {
				if (uv.x >= 0 && uv.x <= 1 && uv.y >= 0 && uv.y <= 1) {
					int z = (int)(CalPlanePointZ(v1.Pos, v2.Pos, v3.Pos, VECTOR4(x, scanlineY, 1, 1))*10000000);//计算该像素的深度值
					if (z < this->depthstencilview->texture2d->data[x][scanlineY].r) {//如果该深度值小于z缓冲区的深度值， 说明应该显示
						this->depthstencilview->texture2d->data[x][scanlineY].r = z;//把z存入z缓冲区
						this->rendertargetview->texture2d->data[x][scanlineY] = PixelShader(uv, Normal, Pos, TBN);//正常绘制模式
						//this->rendertargetview->texture2d->data[x][scanlineY] = PixelShader(uv, Normal, Pos, TBN)+ this->rendertargetview->texture2d->data[x][scanlineY];//add混合， 检查重绘漏绘制
					}
				}
			}
		}
		curx1 = curx1 + invslope1;
		curx2 = curx2 + invslope2;
	}
}
void DeviceContext::fillTopFlatTriangle(SimpleVertex v1, SimpleVertex v2, SimpleVertex v3)
{

	double invslope1 = (v3.Pos.x - v1.Pos.x) / (v3.Pos.y - v1.Pos.y);
	double invslope2 = (v3.Pos.x - v2.Pos.x) / (v3.Pos.y - v2.Pos.y);
	float curx1 = v3.Pos.x;
	float curx2 = v3.Pos.x;

	if (invslope1 < invslope2) {//invslope1,v1代表左边的边
		float temp;
		temp = invslope1;
		invslope1 = invslope2;
		invslope2 = temp;
		SimpleVertex temp1;
		temp1 = v1;
		v1 = v2;
		v2 = temp1;
	}
	VECTOR4 dx = VECTOR4((-v3.Pos.y + v2.Pos.y) / 2, (-v1.Pos.y + v3.Pos.y) / 2, (-v2.Pos.y + v1.Pos.y) / 2, 1) / CalTriangleArea(v1.Pos, v2.Pos, v3.Pos);
	VECTOR4 dy = VECTOR4((-v2.Pos.x + v3.Pos.x) / 2, (-v3.Pos.x + v1.Pos.x) / 2, (-v1.Pos.x + v2.Pos.x) / 2, 1) / CalTriangleArea(v1.Pos, v2.Pos, v3.Pos);
	float dxdenominator = dx.x * v1.Pos.w + dx.y * v2.Pos.w + dx.z * v3.Pos.w;
	float dydenominator = dy.x * v1.Pos.w + dy.y * v2.Pos.w + dy.z * v3.Pos.w;

	//初始uv
	VECTOR4 uv = v3.uv;
	VECTOR4 uvnumerator = v3.uv * v3.Pos.w;
	float uvdenominator = v3.Pos.w;
	//考虑透视矫正算uv增量
	VECTOR4 uvdxnnumerator = v1.uv * v1.Pos.w * dx.x + v2.uv * v2.Pos.w * dx.y + v3.uv * v3.Pos.w * dx.z;//uv在x增量
	VECTOR4 uvdynnumerator = v1.uv * v1.Pos.w * dy.x + v2.uv * v2.Pos.w * dy.y + v3.uv * v3.Pos.w * dy.z;//uv在y增量

	//初始normal
	VECTOR4 Normal = v3.Normal;
	VECTOR4 Normalnumerator = v3.Normal * v3.Pos.w;
	float Normaldenominator = v3.Pos.w;
	//考虑透视矫正算normal增量
	VECTOR4 Normaldxnnumerator = v1.Normal * v1.Pos.w * dx.x + v2.Normal * v2.Pos.w * dx.y + v3.Normal * v3.Pos.w * dx.z;//normal在x增量
	VECTOR4 Normaldynnumerator = v1.Normal * v1.Pos.w * dy.x + v2.Normal * v2.Pos.w * dy.y + v3.Normal * v3.Pos.w * dy.z;//normal在y增量

	//初始世界坐标
	VECTOR4 Pos = v3.PosWorld;
	VECTOR4 Posnumerator = v3.PosWorld * v3.Pos.w;
	float Posdenominator = v3.Pos.w;
	//考虑透视矫正算世界坐标增量
	VECTOR4 Posdxnnumerator = v1.PosWorld * v1.Pos.w * dx.x + v2.PosWorld * v2.Pos.w * dx.y + v3.PosWorld * v3.Pos.w * dx.z;//世界坐标在x增量
	VECTOR4 Posdynnumerator = v1.PosWorld * v1.Pos.w * dy.x + v2.PosWorld * v2.Pos.w * dy.y + v3.PosWorld * v3.Pos.w * dy.z;//世界坐标在y增量

	//初始TBN矩阵
	MATRIX TBN = v3.TBN;
	MATRIX TBNnumerator = v3.TBN * v3.Pos.w;
	float TBNdenominator = v3.Pos.w;
	//考虑透视矫正算TBN增量
	MATRIX TBNdxnnumerator = v1.TBN * v1.Pos.w * dx.x + v2.TBN * v2.Pos.w * dx.y + v3.TBN * v3.Pos.w * dx.z;//TBN在x增量
	MATRIX TBNdynnumerator = v1.TBN * v1.Pos.w * dy.x + v2.TBN * v2.Pos.w * dy.y + v3.TBN * v3.Pos.w * dy.z;//TBN在y增量

	float ymin = max(v1.Pos.y, v2.Pos.y);
	for (int scanlineY = floor(v3.Pos.y); scanlineY >= floor(ymin); scanlineY--) {
		uvnumerator = v3.uv * v3.Pos.w + uvdynnumerator * (scanlineY - v3.Pos.y) + uvdxnnumerator * (scanlineY - v3.Pos.y) * invslope1;//uv
		uvdenominator = v3.Pos.w + dydenominator * (scanlineY - v3.Pos.y) + dxdenominator * (scanlineY - v3.Pos.y) * invslope1;
		uv = uvnumerator / uvdenominator;

		Normalnumerator = v3.Normal * v3.Pos.w + Normaldynnumerator * (scanlineY - v3.Pos.y) + Normaldxnnumerator * (scanlineY - v3.Pos.y) * invslope1;//normal
		Normaldenominator = v3.Pos.w + dydenominator * (scanlineY - v3.Pos.y) + dxdenominator * (scanlineY - v3.Pos.y) * invslope1;
		Normal = Normalnumerator / Normaldenominator;

		Posnumerator = v3.PosWorld * v3.Pos.w + Posdynnumerator * (scanlineY - v3.Pos.y) + Posdxnnumerator * (scanlineY - v3.Pos.y) * invslope1;//pos
		Posdenominator = v3.Pos.w + dydenominator * (scanlineY - v3.Pos.y) + dxdenominator * (scanlineY - v3.Pos.y) * invslope1;
		Pos = Posnumerator / Posdenominator;

		TBNnumerator = v3.TBN * v3.Pos.w + TBNdynnumerator * (scanlineY - v3.Pos.y) + TBNdxnnumerator * (scanlineY - v3.Pos.y) * invslope1;//TBN
		TBNdenominator = v3.Pos.w + dydenominator * (scanlineY - v3.Pos.y) + dxdenominator * (scanlineY - v3.Pos.y) * invslope1;
		TBN = TBNnumerator / TBNdenominator;

		for (int x = floor(curx1); x < floor(curx2); x++) {
			uvnumerator = uvnumerator + uvdxnnumerator;
			uvdenominator = uvdenominator + dxdenominator;
			uv = uvnumerator / uvdenominator;

			Normalnumerator = Normalnumerator + Normaldxnnumerator;
			Normaldenominator = Normaldenominator + dxdenominator;
			Normal = Normalnumerator / Normaldenominator;

			Posnumerator = Posnumerator + Posdxnnumerator;
			Posdenominator = Posdenominator + dxdenominator;
			Pos = Posnumerator / Posdenominator;

			TBNnumerator = TBNnumerator + TBNdxnnumerator;
			TBNdenominator = TBNdenominator + dxdenominator;
			TBN = TBNnumerator / TBNdenominator;

			if (x > 0 && x < 1024 && scanlineY>0 && scanlineY < 768) {

				if (uv.x >= 0 && uv.x <= 1 && uv.y >= 0 && uv.y <= 1) {

					int z = (int)(CalPlanePointZ(v1.Pos, v2.Pos, v3.Pos, VECTOR4(x, scanlineY, 1, 1)) * 10000000);//计算该像素的深度值
					if (z < this->depthstencilview->texture2d->data[x][scanlineY].r) {//如果该深度值小于z缓冲区的深度值， 说明应该显示
						
						this->depthstencilview->texture2d->data[x][scanlineY].r = z;//把z存入z缓冲区
						this->rendertargetview->texture2d->data[x][scanlineY] = PixelShader(uv, Normal, Pos, TBN);//正常绘制模式
						//this->rendertargetview->texture2d->data[x][scanlineY] = PixelShader(uv, Normal, Pos, TBN) + this->rendertargetview->texture2d->data[x][scanlineY];//add混合， 检查重绘漏绘制
					}
				}
			}
		}
		curx1 = curx1 - invslope1;
		curx2 = curx2 - invslope2;
	}
}
void DeviceContext::drawTriangle(SimpleVertex v1, SimpleVertex v2, SimpleVertex v3)
{
	SimpleVertex temp;//保证v1在最上面，v2中间
	if (v1.Pos.y > v2.Pos.y) {
		temp = v1;
		v1 = v2;
		v2 = temp;
	}
	if (v1.Pos.y > v3.Pos.y) {
		temp = v1;
		v1 = v3;
		v3 = temp;
	}
	if (v2.Pos.y > v3.Pos.y) {
		temp = v2;
		v2 = v3;
		v3 = temp;
	}
	if (v2.Pos.y == v3.Pos.y) {//判断是不是上三角形
		fillBottomFlatTriangle(v1, v2, v3);
	}
	else if (v1.Pos.y == v2.Pos.y) {//判断是不是下三角形
		fillTopFlatTriangle(v1, v2, v3);
	}
	else
	{
		fillBottomFlatTriangle(v1, v2, v3);
		fillTopFlatTriangle(v2, v1, v3);
	}
}
void DeviceContext::ClearRenderTargetView()
{
	for (int x = 0; x < 1024; x++) {
		for (int y = 0; y < 768; y++) {
			this->rendertargetview->texture2d->data[x][y] = COLOR(0, 0, 0, 0);
		}
	}
}
void DeviceContext::ClearDepthView()
{
	for (int x = 0; x < 1024; x++) {
		for (int y = 0; y < 768; y++) {
			this->depthstencilview->texture2d->data[x][y] = COLOR(INT_MAX, 0, 0, 0);
		}
	}
}
void DeviceContext::Clearnormaltexture()
{
	for (int x = 0; x < 1024; x++) {
		for (int y = 0; y < 768; y++) {
			this->normaltexturesource.data[x][y] = COLOR(127, 127, 255, 0);
		}
	}
}
void DeviceContext::ClearState()
{

}
void DeviceContext::Release()
{

}

SimpleVertex DeviceContext::VertexShader(SimpleVertex input)
{
	VECTOR4 temp;
	SimpleVertex vertex = input;
	vertex.PosWorld = input.Pos;
	ConstantBuffer* cb = (ConstantBuffer*)this->constantbuffer->data.pSyMem;

	vertex.Pos = Vec4Transform(&(vertex.Pos), &cb->World);
	vertex.Pos = Vec4Transform(&(vertex.Pos), &cb->View);
	vertex.Pos = Vec4Transform(&(vertex.Pos), &cb->Projection);

	temp = Vec4Transform(&(vertex.tangent), &cb->World);
	VECTOR4 T = Vec4Normalize(&temp);
	temp = Vec4Transform(&(vertex.bitangent), &cb->World);
	VECTOR4 B = Vec4Normalize(&temp);
	temp = Vec4Transform(&(vertex.Normal), &cb->World);
	VECTOR4 N = Vec4Normalize(&temp);
	MATRIX mat = MATRIX(T, B, N);
	vertex.TBN = mat;

	return vertex;
}
COLOR DeviceContext::PixelShader(VECTOR4 texcoord, VECTOR4 normal, VECTOR4 pos, MATRIX TBN)
{
	VECTOR4 temp;
	COLOR output;
	ConstantBuffer* cb = (ConstantBuffer*)this->constantbuffer->data.pSyMem;

	int textwidth = this->texturesource.width;
	int textheight = this->texturesource.height;
	VECTOR4 nMap = Color2Vec4(&this->texturesource.data[int(textwidth * texcoord.x)][int(textheight * texcoord.y)]) / 255 * 2 - VECTOR4(1, 1, 1, 1);//读取法线贴图得到法线
	//temp = Vec4Transform(&nMap, &TBN);
	//VECTOR4 N = Vec4Normalize(&temp);

	temp = (cb->lightPos) - pos;
	VECTOR4 L = Vec4Normalize(&temp);
	VECTOR4 N = Vec4Normalize(&normal);//不读法线贴图的法线
	temp = *(cb->eye) - pos;
	VECTOR4 V = Vec4Normalize(&temp);
	VECTOR4 R = (VECTOR4(0, 0, 0, 0) - L) - N * 2 * Vec4Dot(&L, &N);

	cb->id = COLOR(0, 200, 0, 0);
	//cb->id = this->texturesource.data[int(textwidth * texcoord.x)][int(textheight * texcoord.y)];//读取贴图作为漫反射
	COLOR ip;
	if (Vec4Dot(&L, &N) < 0) {
		//ip = (cb->ia) * (cb->ka) + COLOR(0, 0, 0, 0) + (cb->is) * pow(Vec4Dot(&R, &V), cb->a) * (cb->ks);
		ip = (cb->ia) * (cb->ka) + (cb->is) * pow(Vec4Dot(&R, &V), cb->a) * (cb->ks);
	}
	else
	{
		ip = (cb->ia) * (cb->ka) + (cb->id)*Vec4Dot(&L, &N)*(cb->kd) + (cb->is) * pow(Vec4Dot(&R, &V), cb->a) * (cb->ks);
	}
	return ip;
}
HRESULT SwapChain::GetBuffer(UINT Buffer, Texture2D** ppSurface)
{
	*ppSurface = new Texture2D();
	this->surface = *ppSurface;
	return S_OK;
}
void SwapChain::Present()
{

}
void SwapChain::Release()
{

}
void Texture2D::LoadTexture(LPCWSTR path, int width, int height)
{
	this->width = width;
	this->height = height;
	HBITMAP bitmap = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	HDC hdc = CreateCompatibleDC(NULL);
	SelectObject(hdc, bitmap);

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			COLORREF color = GetPixel(hdc, i, j);
			int r = GetRValue(color);
			int g = GetGValue(color);
			int b = GetBValue(color);
			COLOR c(r, g, b, 1);
			data[i][j] = c;
		}
	}
	std::cout << "Load texture" << std::endl;
}
int Texture2D::readTGA(std::string filename)
{
	FILE* filetga;
	fopen_s(&filetga, filename.c_str(), "rb");

	//读取文件头
	TGAHeader data_header;
	//直接读取会有结构体的对齐问题
	fread(&data_header.idLength, sizeof(uint8_t), 1, filetga);
	fread(&data_header.colormapType, sizeof(uint8_t), 1, filetga);
	fread(&data_header.imageType, sizeof(uint8_t), 1, filetga);
	fread(&data_header.colormapOrigin, sizeof(uint16_t), 1, filetga);
	fread(&data_header.colormapLength, sizeof(uint16_t), 1, filetga);
	fread(&data_header.colormapDepth, sizeof(uint8_t), 1, filetga);
	fread(&data_header.xOrigin, sizeof(uint16_t), 1, filetga);
	fread(&data_header.yOrigin, sizeof(uint16_t), 1, filetga);
	fread(&data_header.width, sizeof(uint16_t), 1, filetga);
	fread(&data_header.height, sizeof(uint16_t), 1, filetga);
	fread(&data_header.bitsPerPixel, sizeof(uint8_t), 1, filetga);
	fread(&data_header.imageDescriptor, sizeof(uint8_t), 1, filetga);

	this->width = data_header.width;
	this->height = data_header.height;
	if (data_header.bitsPerPixel != 32) {//只考虑32位的数据
		return 0;
	}
	//像素数据从尺寸
	int datasize = data_header.width * data_header.height * 4;
	//读取颜色数据
	uint8_t* data_color = new uint8_t[datasize];
	fread(data_color, sizeof(uint8_t), datasize, filetga);

	for (int h = 0; h < data_header.height; h++) {
		for (int w = 0; w < data_header.width; w++) {
			int index = data_header.width * h + w;
			data[w][h].r = (int)data_color[index * 4 + 2];//R
			data[w][h].g = (int)data_color[index * 4 + 1];//G
			data[w][h].b = (int)data_color[index * 4 + 0];//B
			data[w][h].w = (int)data_color[index * 4 + 3];//A
		}
	}
}
HRESULT Renderer::CreateDeviceAndSwapChain(SWAP_CHAIN_DESC* pSwapChainDesc, SwapChain** ppSwapChain, Device** ppDevice, DeviceContext** ppContext)
{
	*ppContext = new DeviceContext();
	*ppDevice = new Device();
	*ppSwapChain = new SwapChain();
	this->devicecontext = *ppContext;
	this->device = *ppDevice;
	this->swapchain = *ppSwapChain;
	this->swapchain->desc = *pSwapChainDesc;
	return S_OK;
}
void DDA_Line(int x1, int x2, int y1, int y2, COLOR color1, COLOR color2, DeviceContext* devicecontext)//DDA画线算法
{
	float increaseX = 0, increaseY = 0;//x，y方向的增量
	COLOR increaseColor;//color的增量
	float steps;//循环次数
	float x = 0, y = 0;
	COLOR color;
	steps = max(abs(x2 - x1), abs(y2 - y1));
	if (steps != 0) {
		increaseX = (x2 - x1) / steps;
		increaseY = (y2 - y1) / steps;
		increaseColor = (color2 - color1) / steps;
	}
	x = x1;
	y = y1;
	color = color1;
	for (int i = 0; i < steps; i++) {
		if (x > 0 && x < 1023 && y>0 && y < 767) {
			devicecontext->rendertargetview->texture2d->data[int(x + 0.5)][int(y + 0.5)] = color;
		}
		x += increaseX;
		y += increaseY;
		color = color + increaseColor;
	}
}
bool isPointInTrangle(VECTOR4 point, VECTOR4 a, VECTOR4 b, VECTOR4 c)//叉乘判断点是否在三角形内部
{
	VECTOR3 temp;

	VECTOR3 a3 = Vec4toVec2d(a);
	VECTOR3 b3 = Vec4toVec2d(b);
	VECTOR3 c3 = Vec4toVec2d(c);
	VECTOR3 point3 = Vec4toVec2d(point);
	
	VECTOR3 ab = b3 - a3;
	VECTOR3 bc = c3 - b3;
	VECTOR3 ca = a3 - c3;
	VECTOR3 ap = point3 - a3;
	VECTOR3 bp = point3 - b3;
	VECTOR3 cp = point3 - c3;

	temp = Vec3Cross(&ab, &ap);
	VECTOR3 n1 = Vec3Normalize(&temp);
	temp = Vec3Cross(&bc, &bp);
	VECTOR3 n2 = Vec3Normalize(&temp);
	temp = Vec3Cross(&ca, &cp);
	VECTOR3 n3 = Vec3Normalize(&temp);

	float dotValue1 = Vec3Dot(&n1, &n2);
	float dotValue2 = Vec3Dot(&n2, &n3);
	float dotValue3 = Vec3Dot(&n3, &n1);

	return (dotValue1 == 1) && (dotValue2 == 1) && (dotValue3 == 1);
}
SimpleVertex wviewport(SimpleVertex input)
{
	SimpleVertex ver = input;
	ver.Pos = Wnormalization(&(ver.Pos));
	ver.Pos.x = int((ver.Pos.x + 1) * 1024 / 2 + 0.5);
	ver.Pos.y = int((ver.Pos.y + 1) * 768 / 2 + 0.5);
	return ver;
}
bool ClipSpaceCull(VECTOR4 v1, VECTOR4 v2, VECTOR4 v3, float Near, float Far)//是否三角形至少有一部分在屏幕内
{
	if (v1.w < Near && v2.w < Near && v2.w < Near) {
		return false;
	}
	if (v1.w >Far && v2.w > Far && v2.w > Far) {
		return false;
	}
	if (v1.x > v1.w && v2.x > v2.w && v2.x > v3.w) {
		return false;
	}
	if (v1.x < -v1.w && v2.x < -v2.w && v2.x < -v3.w) {
		return false;
	}
	if (v1.y > v1.w && v2.y > v2.w && v2.y > v3.w) {
		return false;
	}
	if (v1.y < -v1.w && v2.y < -v2.w && v2.y < -v3.w) {
		return false;
	}
	if (v1.z > v1.w && v2.z > v2.w && v2.z > v3.w) {
		return false;
	}
	if (v1.z < -v1.w && v2.z < -v2.w && v2.z < -v3.w) {
		return false;
	}
	return true;
}
bool FaceCull(VECTOR4 v1, VECTOR4 v2, VECTOR4 v3)//是否背面剔除
{
	VECTOR3 temp;
	VECTOR3 tmp1 = VECTOR3(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
	VECTOR3 tmp2 = VECTOR3(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);
	temp = Vec3Cross(&tmp1, &tmp2);
	VECTOR3 normal = Vec3Normalize(&temp);
	VECTOR3 view = VECTOR3(0, 0, 1);
	return Vec3Dot(&normal, &view) < 0;
}
bool Inside(VECTOR4 line, VECTOR4 p)//判断点是否在平面内，通过看点和平面的距离d，d>0则在平面法向量所指的区域内
{
	return (line.x * p.x + line.y * p.y + line.z * p.z + line.w * p.w >= 0);
}
SimpleVertex Intersect(SimpleVertex v1, SimpleVertex v2, VECTOR4 line)//求点与线段的交点，通过端点插值
{
	float da = v1.Pos.x * line.x + v1.Pos.y * line.y + v1.Pos.z * line.z + line.w * v1.Pos.w;
	float db = v2.Pos.x * line.x + v2.Pos.y * line.y + v2.Pos.z * line.z + line.w * v2.Pos.w;
	float weight = da / (da - db);

	SimpleVertex res;
	res.Color = v1.Color + (v2.Color - v1.Color) * weight;
	res.Pos = v1.Pos + (v2.Pos - v1.Pos) * weight;
	res.Normal = v1.Normal + (v2.Normal - v1.Normal) * weight;
	res.uv = v1.uv + (v2.uv - v1.uv) * weight;
	return res;
}
bool AllVertexInside(Triangle input, float Near, float Far)
{
	for (int i = 0; i < 3; i++) {
		if (input.vertex[i].Pos.w<Near || input.vertex[i].Pos.w > Far || input.vertex[i].Pos.x > input.vertex[i].Pos.w || input.vertex[i].Pos.x < -input.vertex[i].Pos.w || input.vertex[i].Pos.y > input.vertex[i].Pos.w || input.vertex[i].Pos.y < -input.vertex[i].Pos.w || input.vertex[i].Pos.z > input.vertex[i].Pos.w || input.vertex[i].Pos.z < -input.vertex[i].Pos.w ) {
			return false;
		}
	}
	return true;
}
std::vector<Triangle> SutherlandHodgeman(std::vector<Triangle> input, float Near, float Far)//输入当前帧的所有三角形列表， 输出裁剪后的三角形列表
{
	const VECTOR4 viewlines[6] = { VECTOR4(0, 0, 1, 1), VECTOR4(0, 0, -1, 1), VECTOR4(1, 0, 0, 1), VECTOR4(0, 1, 0, 1), VECTOR4(-1, 0, 0, 1), VECTOR4(0, -1, 0, 1) };//6个裁剪平面的法向量
	std::vector<Triangle> vec1 = input;
	std::vector<Triangle> vec2;
	std::vector<SimpleVertex> vertex;//存每次裁剪三角形后的顶点
	int inputsize = vec1.size();
	for (int m = 0; m < 6; m++) {//遍历六次裁剪面
		vec2.clear();
		inputsize = vec1.size();
		for (int n = 0; n < inputsize; n++) {//遍历所有三角形
			if (AllVertexInside(vec1[n], Near, Far)) {//如果点全在范围内，则不用任何裁剪，直接输出
				vec2.push_back(vec1[n]);
			}
			else 
			{
				for (int j = 0; j < 3; j++) {//遍历三角形的三个顶点
					SimpleVertex current = vec1[n].vertex[j];//当前的点
					SimpleVertex last = vec1[n].vertex[(j + 3 - 1) % 3];//上一个点

					if (Inside(viewlines[m], current.Pos)) {//如果当前的点在裁剪平面内
						if (!Inside(viewlines[m], last.Pos)) {//如果上一个点不在平面内
							SimpleVertex intersecting = Intersect(last, current, viewlines[m]);
							vertex.push_back(intersecting);
						}
						vertex.push_back(current);
					}
					else if (Inside(viewlines[m], last.Pos)) {//虽然当前的点不在平面内，但是上一个点在平面内
						SimpleVertex intersecting = Intersect(last, current, viewlines[m]);
						vertex.push_back(intersecting);
					}
				}

				int size = vertex.size() - 2;//根据顶点装配三角形，生成新的三角形，传入vec2
				for (int k = 0; k < size; k++) {
					Triangle triangle;
					triangle.vertex[0] = vertex[0];
					triangle.vertex[1] = vertex[k + 1];
					triangle.vertex[2] = vertex[k + 2];
					vec2.push_back(triangle);
				}
				vertex.clear();
			}
		}
		vec1 = vec2;
	}
	return vec1;
}
void StringSplit(std::string s, char splitchar, std::vector<std::string>& vec)
{
	if (vec.size() > 0) {//清空vec
		vec.clear();
	}
	int length = s.length();
	int start = 0;
	for (int i = 0; i < length; i++) {
		if (s[i] == splitchar && i == 0) {//第一个就遇到分割符
			start += 1;
		}
		else if (s[i] == splitchar) {
			vec.push_back(s.substr(start, i - start));
			start = i + 1;
		}
		else if (i == length - 1) {
			vec.push_back(s.substr(start, i + 1 - start));
		}
	}
}
void ReadObjFile(std::string path, Mesh* obj)
{
	std::ifstream in(path);
	std::string txt = "";
	if (in) {//获取文件成功
		std::cout << "获取文件成功"<<std::endl;
		while (getline(in, txt))//line中不包括每行的换符行
		{
			//顶点坐标
			if (txt[0] == 'v' && txt[1] == ' ') {
				std::vector<std::string> num;
				txt.erase(0, 2);
				StringSplit(txt, ' ', num);
				VECTOR4 pos;
				pos = VECTOR4((float)atof(num[0].c_str()), (float)atof(num[1].c_str()), (float)atof(num[2].c_str()), 1);
				obj->positionBuffer.push_back(pos);
			}
			//顶点法线
			else if (txt[0] == 'v' && txt[1] == 'n') {
				std::vector<std::string> num;
				txt.erase(0, 3);
				StringSplit(txt, ' ', num);
				VECTOR4 n;
				n = VECTOR4((float)atof(num[0].c_str()), (float)atof(num[1].c_str()), (float)atof(num[2].c_str()), 0);
				obj->normalBuffer.push_back(n);
			}
			//uv
			else if (txt[0] == 'v' && txt[1] == 't') {
				std::vector<std::string> num;
				txt.erase(0, 3);
				StringSplit(txt, ' ', num);
				VECTOR4 uv;
				uv = VECTOR4((float)atof(num[0].c_str()), (float)atof(num[1].c_str()), 0, 0);
				obj->uvBuffer.push_back(uv);
			}
			//索引编号
			//在面的数据中多了贴图坐标uv和法线的索引号，索引号分别用左斜线（/）隔开
			else if (txt[0] == 'f' && txt[1] == ' ') {
				std::vector<std::string> num;
				txt.erase(0, 2);
				StringSplit(txt, ' ', num);
				std::vector<VECTOR4> indexesVector;//存的是模型文件一行的所有索引
				for (int i = 0; i < num.size(); i++) {
					std::vector<std::string> threeIndex;
					StringSplit(num[i], '/', threeIndex);
					VECTOR4 indexes = { (float)atoi(threeIndex[0].c_str()), (float)atoi(threeIndex[1].c_str()),(float)atoi(threeIndex[2].c_str()),0 };
					indexesVector.push_back(indexes);
				}
				obj->indexBuffer.push_back(indexesVector);
			}
		}
	}
	else {//没有该文件
		std::cout << "no file" << std::endl;
	}
}