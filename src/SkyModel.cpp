// SkyModel.cpp: implementation of the SkyModel class.
//
//////////////////////////////////////////////////////////////////////

//#include "DXTemplate.h"
#include "SkyModel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SkyModel::SkyModel()
{
	mesh = NULL;
	material = NULL;
	joint = NULL;
	meshvbinfo = NULL;

	m_num_mesh_vertices = 0;
	m_num_mesh_triangles = 0;
	m_num_joints = 0;
	m_BoundingRadius = 0.0f;
	Cx = 0.0f; Cy = 0.0f; Cz = 0.0f;
	m_isAnimation = false;	
	VAngle = 3.1415f * 0.25f;
}

SkyModel::~SkyModel()
{
	if(material != NULL)
	{
		if(m_num_materials != 0)
			delete[] material;

		material = NULL;
	}
	if(meshvbinfo != NULL)
	{
		delete[] meshvbinfo;
		meshvbinfo = NULL;
	}

	if(joint != NULL)
	{
		delete[] joint;
		joint = NULL;
	}
}

bool SkyModel::LoadASCIIFile(char *filename)
{
	char szLine[256];
	int i, j, k;
	int num_shapes, num_vertices, num_normals, num_triangles, num_materials, num_joints;
	
	m_file = fopen (filename, "rt");
		if (!m_file)
		{
			//MessageBox("File open error!", "Error", MB_OK);
			return false;
		}

	while (fgets(szLine, 256, m_file) != NULL)
	{
		//Read meshes.
		if (sscanf (szLine, "# of meshes: %d", &num_shapes) == 1)
		{
			mesh = new Mesh[num_shapes];
			m_num_meshes = num_shapes;

			if (!fgets (szLine, 256, m_file))
				return false;

			//Loop through all meshes.
			for(i = 0; i < num_shapes; i++)
			{

				if (!fgets (szLine, 256, m_file))
					return false;
				if (sscanf (szLine, "Mesh name: \"%[^\"]\"", mesh[i].Name) != 1)
					return false;

				if (!fgets (szLine, 256, m_file))
					return false;
				if (sscanf (szLine, "Material index: %d", &mesh[i].material_index) != 1)
					return false;
				
				//Vertices.
				if (!fgets (szLine, 256, m_file))
					return false;
				if (sscanf (szLine, "# of vertices: %d", &num_vertices) != 1)
					return false;

				mesh[i].vertice = new Vertice[num_vertices];
				mesh[i].num_vertices = num_vertices;
				for (j = 0; j < num_vertices; j++)
				{
					if (!fgets (szLine, 256, m_file))
						return false;
					if (sscanf (szLine, "%f %f %f %f %f %d",
						&mesh[i].vertice[j].x, &mesh[i].vertice[j].y, &mesh[i].vertice[j].z,
						&mesh[i].vertice[j].u, &mesh[i].vertice[j].v, &mesh[i].vertice[j].joint_index
						) != 6)
						return false;

					// adjust the y direction of the texture coordinate
					//mesh[i].vertice[j].v = 1.0f - mesh[i].vertice[j].v;
				}//End read vertices.

				//Normals.
				if (!fgets (szLine, 256, m_file))
					return false;
				if (sscanf (szLine, "# of normals: %d", &num_normals) != 1)
					return false;

				mesh[i].normal = new Normal[num_normals];
				mesh[i].num_normals = num_normals;
				for (j = 0; j < num_normals; j++)
				{
					if (!fgets (szLine, 256, m_file))
						return false;
					if (sscanf (szLine, "%f %f %f", 
						&mesh[i].normal[j].x, &mesh[i].normal[j].y, &mesh[i].normal[j].z) != 3)
						return false;
				}

				//Index.
				if (!fgets (szLine, 256, m_file))
					return false;
				if (sscanf (szLine, "# of triangles: %d", &num_triangles) != 1)
					return false;

				mesh[i].index = new Index[num_triangles];
				mesh[i].num_triangles = num_triangles;
				for (j = 0; j < num_triangles; j++)
				{
					if (!fgets (szLine, 256, m_file))
						return false;

					if (sscanf (szLine, "%d %d %d %d %d %d",
						&mesh[i].index[j].v[0], &mesh[i].index[j].v[1], &mesh[i].index[j].v[2],
						&mesh[i].index[j].n[0], &mesh[i].index[j].n[1], &mesh[i].index[j].n[2]
						) != 6)
						return false;
				}

				m_num_mesh_vertices = m_num_mesh_vertices + num_triangles * 3;
				m_num_mesh_triangles += num_triangles;

				if (!fgets (szLine, 256, m_file))
					return false;
			}
			continue;
		}

        if (sscanf (szLine, "# of materials: %d", &num_materials) == 1)
        {
			m_num_materials = num_materials;
			material = new Material[num_materials];
           for (i = 0; i < num_materials; i++)
            {
				if (!fgets (szLine, 256, m_file))
					return false;

				// name
				if (!fgets (szLine, 256, m_file))
					return false;
				if (sscanf (szLine, "Material name: \"%[^\"]\"", material[i].Name) != 1)
					return false;

				// ambient
				if (!fgets (szLine, 256, m_file))
					return false;

				if (sscanf (szLine, "%f %f %f %f", &material[i].Ambient[0], &material[i].Ambient[1], &material[i].Ambient[2], &material[i].Ambient[3]) != 4)
					return false;

				// diffuse
				if (!fgets (szLine, 256, m_file))
					return false;
				if (sscanf (szLine, "%f %f %f %f", &material[i].Diffuse[0], &material[i].Diffuse[1], &material[i].Diffuse[2], &material[i].Diffuse[3]) != 4)
					return false;

				// specular
				if (!fgets (szLine, 256, m_file))
					return false;
				if (sscanf (szLine, "%f %f %f %f", &material[i].Specular[0], &material[i].Specular[1], &material[i].Specular[2], &material[i].Specular[3]) != 4)
					return false;

				// emissive
				if (!fgets (szLine, 256, m_file))
					return false;
				if (sscanf (szLine, "%f %f %f %f", &material[i].Emissive[0], &material[i].Emissive[1], &material[i].Emissive[2], &material[i].Emissive[3]) != 4)
					return false;

				// shininess
				if (!fgets (szLine, 256, m_file))
					return false;
				if (sscanf (szLine, "%f", &material[i].Shininess) != 1)
					return false;

				// transparency
				if (!fgets (szLine, 256, m_file))
					return false;
				if (sscanf (szLine, "%f", &material[i].Transparency) != 1)
					return false;

				// diffuse texture
				if (!fgets (szLine, 256, m_file))
					return false;
				strcpy(material[i].DiffuseTexture, "");
				sscanf (szLine, "Diffuse texture file: \"%[^\"]\"", material[i].DiffuseTexture);
				strcpy(TextureFile, material[i].DiffuseTexture);
				
				if(strcmp(material[i].DiffuseTexture, "") == 0)
					material[i].hasDiffuseTexture = false;
				else
					material[i].hasDiffuseTexture = true;

				// alpha texture
				if (!fgets (szLine, 256, m_file))
					return false;
				strcpy(material[i].AlphaTexture, "");
				sscanf (szLine, "Diffuse alpha file: \"%[^\"]\"", material[i].AlphaTexture);
				
				if(strcmp(material[i].AlphaTexture, "") == 0)
					material[i].hasAlphaTexture = false;
				else
					material[i].hasAlphaTexture = true;
			}
            continue;
        }

		if (sscanf (szLine, "# of joints: %d", &num_joints) == 1)
		{
			m_num_joints = num_joints;
			joint = new Joint[num_joints];

            for (i = 0; i < num_joints; i++)
			{
				if (!fgets (szLine, 256, m_file))
					return false;

				if (!fgets (szLine, 256, m_file))
					return false;
				if (sscanf (szLine, "Joint name: \"%[^\"]\"", joint[i].Name) != 1)
					return false;

				if (!fgets (szLine, 256, m_file))
					return false;
				if (sscanf (szLine, "IsParent: %u", &joint[i].isParent) != 1)
					return false;

				if (!fgets (szLine, 256, m_file))
					return false;
				strcpy(joint[i].ParentName, "");
				sscanf(szLine, "Parent name: \"%[^\"]\"", joint[i].ParentName);

				if (!fgets (szLine, 256, m_file))
					return false;
				if (sscanf (szLine, "Default Translation Key: %f %f %f", &joint[i].DefaultTranslation[0], &joint[i].DefaultTranslation[1], &joint[i].DefaultTranslation[2]) != 3)
					return false;

				if (!fgets (szLine, 256, m_file))
					return false;
				if (sscanf (szLine, "Default Rotation Key: %f %f %f", &joint[i].DefaultRotation[0], &joint[i].DefaultRotation[1], &joint[i].DefaultRotation[2]) != 3)
					return false;

				if (!fgets (szLine, 256, m_file))
					return false;
				if (sscanf (szLine, "# of Translation Keys: %d", &joint[i].num_TKeys) != 1)
					return false;
				
				joint[i].TranslationKey = new KeyFrame[joint[i].num_TKeys];
				for(j = 0; j < joint[i].num_TKeys; j++)
				{
					if (!fgets (szLine, 256, m_file))
						return false;
					if (sscanf (szLine, "%f %f %f %f", &joint[i].TranslationKey[j].FrameTime, &joint[i].TranslationKey[j].KeyValue[0], &joint[i].TranslationKey[j].KeyValue[1], &joint[i].TranslationKey[j].KeyValue[2]) != 4)
						return false;
				}

				if (!fgets (szLine, 256, m_file))
					return false;
				if (sscanf (szLine, "# of Rotation Keys: %d", &joint[i].num_RKeys) != 1)
					return false;
				
				joint[i].RotationKey = new KeyFrame[joint[i].num_RKeys];
				for(j = 0; j < joint[i].num_RKeys; j++)
				{
					if (!fgets (szLine, 256, m_file))
						return false;
					if (sscanf (szLine, "%f %f %f %f", &joint[i].RotationKey[j].FrameTime, &joint[i].RotationKey[j].KeyValue[0], &joint[i].RotationKey[j].KeyValue[1], &joint[i].RotationKey[j].KeyValue[2]) != 4)
						return false;
				}
			}
			m_isAnimation = true;
		}
	}
	fclose(m_file);

	meshvbinfo = new MeshVBInfo[m_num_meshes];
			
	m_MaxX = 0.0f; m_MinX = 0.0f; m_MaxY = 0.0f; m_MinY = 0.0f; m_MaxZ = 0.0f; m_MinZ = 0.0f;

	float var1, var2, var3, var;

	int count = 0;
	//Find bounding sphere radius.			 
	for(i = 0; i < m_num_meshes; i++)
	{
		meshvbinfo[i].StartIndex = count;
		meshvbinfo[i].num_Triangles = mesh[i].num_triangles;

		for(j = 0; j < mesh[i].num_triangles; j++)
		{
			for(k = 0; k < 3; k++)
			{						
				if(mesh[i].vertice[mesh[i].index[j].v[k]].x > m_MaxX)
					m_MaxX = mesh[i].vertice[mesh[i].index[j].v[k]].x;
				if(mesh[i].vertice[mesh[i].index[j].v[k]].x < m_MinX)
					m_MinX = mesh[i].vertice[mesh[i].index[j].v[k]].x;

				if(mesh[i].vertice[mesh[i].index[j].v[k]].y > m_MaxY)
					m_MaxY = mesh[i].vertice[mesh[i].index[j].v[k]].y;
				if(mesh[i].vertice[mesh[i].index[j].v[k]].y < m_MinY)
					m_MinY = mesh[i].vertice[mesh[i].index[j].v[k]].y;

				if(mesh[i].vertice[mesh[i].index[j].v[k]].z > m_MaxZ)
					m_MaxZ = mesh[i].vertice[mesh[i].index[j].v[k]].z;
				if(mesh[i].vertice[mesh[i].index[j].v[k]].z < m_MinZ)
					m_MinZ = mesh[i].vertice[mesh[i].index[j].v[k]].z;

				count++;
			}
		}
	}

	Cx = (m_MaxX + m_MinX) * 0.5f;
	Cy = (m_MaxY + m_MinY) * 0.5f;
	Cz = (m_MaxZ + m_MinZ) * 0.5f;

	for(i = 0; i < m_num_meshes; i++)
	{
		for(j = 0; j < mesh[i].num_triangles; j++)
		{
			for(k = 0; k < 3; k++)
			{
				//var1 = (mesh[i].vertice[mesh[i].index[j].v[k]].x - Cx) * (mesh[i].vertice[mesh[i].index[j].v[k]].x - Cx);
				//var2 = (mesh[i].vertice[mesh[i].index[j].v[k]].y - Cy) * (mesh[i].vertice[mesh[i].index[j].v[k]].y - Cy);
				//var3 = (mesh[i].vertice[mesh[i].index[j].v[k]].z - Cz) * (mesh[i].vertice[mesh[i].index[j].v[k]].z - Cz);

				var1 = (mesh[i].vertice[mesh[i].index[j].v[k]].x) * (mesh[i].vertice[mesh[i].index[j].v[k]].x);
				var2 = (mesh[i].vertice[mesh[i].index[j].v[k]].y) * (mesh[i].vertice[mesh[i].index[j].v[k]].y);
				var3 = (mesh[i].vertice[mesh[i].index[j].v[k]].z) * (mesh[i].vertice[mesh[i].index[j].v[k]].z);

				var = powf(var1 + var2 + var3, 0.5f);

				if(var > m_BoundingRadius)
					m_BoundingRadius = var;
			}
		}
	}
	m_ViewOffset = m_BoundingRadius / (float)tan(VAngle * 0.5f);

	if(m_isAnimation)
	{
		if(!LinkJoints())
		{
			//MessageBox("Can't find the parent!", "Error", MB_OK);
			return false;
		}

		for(i = 0; i < m_num_joints; i++)
			joint[i].InitializeMatrix();
	}

	//Transform vertices to joint's relative transformation coordinate.
	Matrix matTemp;
	int iJoint;
	float v[3];
	for(i = 0; i < m_num_meshes; i++)
	{
		for(j = 0; j < mesh[i].num_vertices; j++)
		{
			iJoint = mesh[i].vertice[j].joint_index;

			//Does vertice belong to joint?
			if(iJoint != -1)
			{
				matTemp.Set(joint[iJoint].FinalMatrix.value);

				v[0] = mesh[i].vertice[j].x;
				v[1] = mesh[i].vertice[j].y;
				v[2] = mesh[i].vertice[j].z;

				v[0] -= matTemp.value[3];
				v[1] -= matTemp.value[7];
				v[2] -= matTemp.value[11];
				matTemp.inverseRotateVector(v);

				//Vertices.
				mesh[i].vertice[j].x = v[0];
				mesh[i].vertice[j].y = v[1];
				mesh[i].vertice[j].z = v[2];
			}
		}
	}

	return true;
}

bool SkyModel::LinkJoints()
{
	int i, j;

	//Link children to parent.

    for (i = 0; i < m_num_joints; i++)            
	{
		//joint[i].Parent = NULL;

		if(!joint[i].isParent)	// does bone have parent?
		{
			for (j = 0; j < m_num_joints; j++)	// search for parent
			{
				if( stricmp(joint[j].Name, joint[i].ParentName) == 0)	// j is parent of i
				{
					joint[i].ptrParent = &joint[j];
					break;
				}
			}
			if (joint[i].ptrParent == NULL) // Unable to find parent bone 
				return false;
        }
    }
	
	return true;
}
