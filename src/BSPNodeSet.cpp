// BSPNodeSet.cpp: implementation of the BSPNodeSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BSPNodeSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BSPNodeSet::BSPNodeSet()
{
	m_numNodes = 0;
	m_numTriangles = 0;
	m_numMaterials = 0;
	m_nRootNode = 0;
	m_nPosChild = 0;
	m_nNegChild = 0;
	m_pBSPNode = NULL;
	m_pMaterial = NULL;
}

BSPNodeSet::~BSPNodeSet()
{
	if(m_pBSPNode != NULL)
		delete[] m_pBSPNode;

	if(m_pMaterial != NULL)
		delete[] m_pMaterial;
}

bool BSPNodeSet::LoadBSPNodeFromFile(char *filename)
{
	FILE *stream;
	int i, j, k;
	char line[512];

	stream = fopen(filename, "r");

	fgets(line, 512, stream);
	sscanf(line, "NumNodes: %d", &m_numNodes);

	fgets(line, 512, stream);
	sscanf(line, "NumTriangles: %d", &m_numTriangles);

	fgets(line, 512, stream);
	sscanf(line, "TreeHead: %d", &m_nRootNode);

	if(m_numNodes <= 0)
		return false;

	m_pBSPNode = new BSP_Node[m_numNodes];

	if(stream != NULL)
	{
		for(i = 0; i < m_numNodes; i++)
		{
			fgets(line, 512, stream);
			sscanf(line, "%d\n", &m_pBSPNode[i].nNode);

			fgets(line, 512, stream);
			sscanf(line, "%d\n", &k);
			if(k == 0)
				m_pBSPNode[i].side = 0;
			else
				m_pBSPNode[i].side = 1;

			fgets(line, 512, stream);
			sscanf(line, "%d\n", &k);
			if(k == 0)
				m_pBSPNode[i].IsLeaf = 0;
			else
				m_pBSPNode[i].IsLeaf = 1;

			fgets(line, 512, stream);
			sscanf(line, "%d\n", &m_pBSPNode[i].Parent_nNode);

			fgets(line, 512, stream);
			sscanf(line, "%d\n", &m_pBSPNode[i].nPosChild);

			fgets(line, 512, stream);
			sscanf(line, "%d\n", &m_pBSPNode[i].nNegChild);

			fgets(line, 512, stream);
			sscanf(line, "%f %f %f %f %f\n", &m_pBSPNode[i].v[0].x, &m_pBSPNode[i].v[0].y, &m_pBSPNode[i].v[0].z, &m_pBSPNode[i].v[0].u, &m_pBSPNode[i].v[0].v);
			fgets(line, 512, stream);
			sscanf(line, "%f %f %f %f %f\n", &m_pBSPNode[i].v[1].x, &m_pBSPNode[i].v[1].y, &m_pBSPNode[i].v[1].z, &m_pBSPNode[i].v[1].u, &m_pBSPNode[i].v[1].v);
			fgets(line, 512, stream);
			sscanf(line, "%f %f %f %f %f\n", &m_pBSPNode[i].v[2].x, &m_pBSPNode[i].v[2].y, &m_pBSPNode[i].v[2].z, &m_pBSPNode[i].v[2].u, &m_pBSPNode[i].v[2].v);

			fgets(line, 512, stream);
			sscanf(line, "%f %f %f\n", &m_pBSPNode[i].normal.x, &m_pBSPNode[i].normal.y, &m_pBSPNode[i].normal.z);

			fgets(line, 512, stream);
			sscanf(line, "%d\n", &m_pBSPNode[i].MaterialIndex);

			fgets(line, 512, stream);
			sscanf(line, "%d\n", &k);
			if(k == 0)
				m_pBSPNode[i].IsTransparent = 0;
			else
				m_pBSPNode[i].IsTransparent = 1;

			// Read coplanar triangles in this node.
			fgets(line, 512, stream);
			sscanf(line, "NumCoplanars: %d\n", &m_pBSPNode[i].numConTri);
			if(m_pBSPNode[i].numConTri > 0)
			{
				m_pBSPNode[i].pCoplanarTriangle = new Triangle[m_pBSPNode[i].numConTri];
				m_pBSPNode[i].pNormal_CPTri = new Vector3D[m_pBSPNode[i].numConTri];
				m_pBSPNode[i].p_nMaterial_CPTri = new int[m_pBSPNode[i].numConTri];
				m_pBSPNode[i].p_IsTransparent_CPTri = new bool[m_pBSPNode[i].numConTri];

				for(j = 0; j < m_pBSPNode[i].numConTri; j++)
				{
					fgets(line, 512, stream);
					sscanf(line, "%f %f %f %f %f\n", &m_pBSPNode[i].pCoplanarTriangle[j].v[0].x, &m_pBSPNode[i].pCoplanarTriangle[j].v[0].y, &m_pBSPNode[i].pCoplanarTriangle[j].v[0].z, &m_pBSPNode[i].pCoplanarTriangle[j].v[0].u, &m_pBSPNode[i].pCoplanarTriangle[j].v[0].v);
					fgets(line, 512, stream);
					sscanf(line, "%f %f %f %f %f\n", &m_pBSPNode[i].pCoplanarTriangle[j].v[1].x, &m_pBSPNode[i].pCoplanarTriangle[j].v[1].y, &m_pBSPNode[i].pCoplanarTriangle[j].v[1].z, &m_pBSPNode[i].pCoplanarTriangle[j].v[1].u, &m_pBSPNode[i].pCoplanarTriangle[j].v[1].v);
					fgets(line, 512, stream);
					sscanf(line, "%f %f %f %f %f\n", &m_pBSPNode[i].pCoplanarTriangle[j].v[2].x, &m_pBSPNode[i].pCoplanarTriangle[j].v[2].y, &m_pBSPNode[i].pCoplanarTriangle[j].v[2].z, &m_pBSPNode[i].pCoplanarTriangle[j].v[2].u, &m_pBSPNode[i].pCoplanarTriangle[j].v[2].v);
					fgets(line, 512, stream);
					sscanf(line, "%f %f %f\n", &m_pBSPNode[i].pNormal_CPTri[j].x, &m_pBSPNode[i].pNormal_CPTri[j].y, &m_pBSPNode[i].pNormal_CPTri[j].z);					
					fgets(line, 512, stream);
					sscanf(line, "%d\n", &m_pBSPNode[i].p_nMaterial_CPTri[j]);
					fgets(line, 512, stream);
					sscanf(line, "%d\n", &k);
					if(k == 0)
						m_pBSPNode[i].p_IsTransparent_CPTri[j] = 0;
					else
						m_pBSPNode[i].p_IsTransparent_CPTri[j] = 1;
				}
			}

			// Read triangles in this node.
			fgets(line, 512, stream);
			sscanf(line, "NumSubNodes: %d\n", &m_pBSPNode[i].numTriangles);
			if(m_pBSPNode[i].numTriangles != 0)
			{
				m_pBSPNode[i].tri = new Triangle[m_pBSPNode[i].numTriangles];
				m_pBSPNode[i].Normal_Leaf = new Vector3D[m_pBSPNode[i].numTriangles];
				m_pBSPNode[i].nMaterial_Leaf = new int[m_pBSPNode[i].numTriangles];
				m_pBSPNode[i].IsTransparent_Leaf = new bool[m_pBSPNode[i].numTriangles];

				for(j = 0; j < m_pBSPNode[i].numTriangles; j++)
				{
					fgets(line, 512, stream);
					sscanf(line, "%f %f %f %f %f\n", &m_pBSPNode[i].tri[j].v[0].x, &m_pBSPNode[i].tri[j].v[0].y, &m_pBSPNode[i].tri[j].v[0].z, &m_pBSPNode[i].tri[j].v[0].u, &m_pBSPNode[i].tri[j].v[0].v);
					fgets(line, 512, stream);
					sscanf(line, "%f %f %f %f %f\n", &m_pBSPNode[i].tri[j].v[1].x, &m_pBSPNode[i].tri[j].v[1].y, &m_pBSPNode[i].tri[j].v[1].z, &m_pBSPNode[i].tri[j].v[1].u, &m_pBSPNode[i].tri[j].v[1].v);
					fgets(line, 512, stream);
					sscanf(line, "%f %f %f %f %f\n", &m_pBSPNode[i].tri[j].v[2].x, &m_pBSPNode[i].tri[j].v[2].y, &m_pBSPNode[i].tri[j].v[2].z, &m_pBSPNode[i].tri[j].v[2].u, &m_pBSPNode[i].tri[j].v[2].v);
					fgets(line, 512, stream);
					sscanf(line, "%f %f %f\n", &m_pBSPNode[i].Normal_Leaf[j].x, &m_pBSPNode[i].Normal_Leaf[j].y, &m_pBSPNode[i].Normal_Leaf[j].z);					
					fgets(line, 512, stream);
					sscanf(line, "%d\n", &m_pBSPNode[i].nMaterial_Leaf[j]);
					fgets(line, 512, stream);
					sscanf(line, "%d\n", &k);
					if(k == 0)
						m_pBSPNode[i].IsTransparent_Leaf[j] = 0;
					else
						m_pBSPNode[i].IsTransparent_Leaf[j] = 1;
				}
			}
		}

		// Read material(s).
		fgets(line, 512, stream);
		sscanf(line, "NumMaterials: %d\n", &m_numMaterials);

		if(m_numMaterials > 0)
		{
			m_pMaterial = new Material[m_numMaterials];

			for(i = 0; i < m_numMaterials; i++)
			{
				fgets(line, 512, stream);
				sscanf(line, "Ambient: %f %f %f %f\n", &m_pMaterial[i].Ambient[0], &m_pMaterial[i].Ambient[1], &m_pMaterial[i].Ambient[2], &m_pMaterial[i].Ambient[3]);
				fgets(line, 512, stream);
				sscanf(line, "Diffuse: %f %f %f %f\n", &m_pMaterial[i].Diffuse[0], &m_pMaterial[i].Diffuse[1], &m_pMaterial[i].Diffuse[2], &m_pMaterial[i].Diffuse[3]);
				fgets(line, 512, stream);
				sscanf(line, "Specular: %f %f %f %f\n", &m_pMaterial[i].Specular[0], &m_pMaterial[i].Specular[1], &m_pMaterial[i].Specular[2], &m_pMaterial[i].Specular[3]);
				fgets(line, 512, stream);
				sscanf(line, "Emissive: %f %f %f %f\n", &m_pMaterial[i].Emissive[0], &m_pMaterial[i].Emissive[1], &m_pMaterial[i].Emissive[2], &m_pMaterial[i].Emissive[3]);
				fgets(line, 512, stream);
				sscanf(line, "Shininess: %f\n", &m_pMaterial[i].Shininess);
				fgets(line, 512, stream);
				sscanf(line, "Transparency: %f\n", &m_pMaterial[i].Transparency);
				fgets(line, 512, stream);
				sscanf(line, "DiffuseTexture: %d %s\n", &k, m_pMaterial[i].DiffuseTexture);
				if(k == 0)
					m_pMaterial[i].hasDiffuseTexture = 0;
				else
					m_pMaterial[i].hasDiffuseTexture = 1;
				fgets(line, 512, stream);
				sscanf(line, "AlphaTexture: %d %s\n", &k, m_pMaterial[i].AlphaTexture);
				if(k == 0)
					m_pMaterial[i].hasAlphaTexture = 0;
				else
					m_pMaterial[i].hasAlphaTexture = 1;
			}
		}

		fclose(stream);
	}
	else
		return false;

	for(i = 0; i < m_numNodes; i++)
	{
		if(m_pBSPNode[i].Parent_nNode == 0)
			m_pBSPNode[i].Parent = NULL;
		else
			m_pBSPNode[i].Parent = &m_pBSPNode[m_pBSPNode[i].Parent_nNode - 1];

		if(m_pBSPNode[i].nPosChild == 0)
			m_pBSPNode[i].Pos_Child = NULL;
		else
			m_pBSPNode[i].Pos_Child = &m_pBSPNode[m_pBSPNode[i].nPosChild - 1];

		if(m_pBSPNode[i].nNegChild == 0)
			m_pBSPNode[i].Neg_Child = NULL;
		else
			m_pBSPNode[i].Neg_Child = &m_pBSPNode[m_pBSPNode[i].nNegChild - 1];	
	}

	return true;
}
