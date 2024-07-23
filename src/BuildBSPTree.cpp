// BuildBSPTree.cpp: implementation of the BuildBSPTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BuildBSPTree.h"
#include "MyMaths.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BuildBSPTree::BuildBSPTree()
{
	headnode = NULL;
	treehead = NULL;
}

BuildBSPTree::~BuildBSPTree()
{

}

bool BuildBSPTree::BuildRoot()
{
	/*
	int num_splits, previous = -1;

	BSP_Node *temp;

	//Sort for minimum splits.
	temp = headnode;
	while(temp != NULL)
	{
		num_splits = NumSplits(temp, true);

		if(previous == -1)
		{
			previous = num_splits;
			treehead = temp;
		}
		else
		{
			if(num_splits < previous)
				treehead = temp;

			previous = num_splits;
		}

		temp = temp->Next;
	}
*/
	BSP_Node *target = NULL;

	target = FindBalanceNode(0, NULL);
	//target = FindMinSplits(0, NULL);

	treehead = target;
	//treehead = headnode;
	if(treehead != NULL)
		BuildBranches(treehead);

	return true;
}

void BuildBSPTree::SetHeadNode(BSP_Node *hn)
{
	headnode = hn;
}

//Using BSP node "node" as splitting plane to split BSP node "temp". 
void BuildBSPTree::UpdateLink(BSP_Node *node, BSP_Node *temp)
{
	int loop, index;
	float var[3];
	Vector2D uv[3], splitted_uv1, splitted_uv2;
	Vector3D v[3];
	
	float A, B, C, D;

	A = node->normal.x;
	B = node->normal.y;
	C = node->normal.z;
	D = -(node->v[2].x * node->normal.x + node->v[2].y * node->normal.y + node->v[2].z * node->normal.z);

	v[0] = Vector3D(temp->v[0].x, temp->v[0].y, temp->v[0].z);
	v[1] = Vector3D(temp->v[1].x, temp->v[1].y, temp->v[1].z);
	v[2] = Vector3D(temp->v[2].x, temp->v[2].y, temp->v[2].z);

	for(loop = 0; loop < 3; loop++)
	{
		var[loop] = A * v[loop].x + B * v[loop].y + C * v[loop].z + D;
		if((float)fabs((float)var[loop]) < 1e-4)
			var[loop] = 0.0f;
	}

	//Check for positive side.
	if(var[0] >= 0.0f && var[1] >= 0.0f && var[2] >= 0.0f)
		temp->side = 0;
	else
	{
		//Check for negative side.
		if(var[0] <= 0.0f && var[1] <= 0.0f && var[2] <= 0.0f)
			temp->side = 1;
		else
		{
			//Triangle is intersecting with plane.
			//Following codes are to split one triangle into three.

			//Find intersection points.
			float t;
			Vector3D p1, p2, p;

			p = Vector3D(node->v[0].x, node->v[0].y, node->v[0].z);

			if(var[0] * var[1] >= 0.0f)
			{
				//v[2].
				index = 2;

				uv[0] = Vector2D(temp->v[2].u, temp->v[2].v);
				uv[1] = Vector2D(temp->v[0].u, temp->v[0].v);
				uv[2] = Vector2D(temp->v[1].u, temp->v[1].v);

				t = ((p - v[0]) * node->normal) / ((v[2] - v[0]) * node->normal);
				p1 = v[0] + (v[2] - v[0]) * t;
				splitted_uv1 = SplitUV(&Vector2D(uv[0].x, uv[0].y), &Vector2D(uv[1].x, uv[1].y), t);

				t = ((p - v[1]) * node->normal) / ((v[2] - v[1]) * node->normal);
				p2 = v[1] + (v[2] - v[1]) * t;
				splitted_uv2 = SplitUV(&Vector2D(uv[0].x, uv[0].y), &Vector2D(uv[2].x, uv[2].y), t);
			}
			else
			{
				if(var[0] * var[2] >= 0.0f)
				{
					//v[1].
					index = 1;

					uv[0] = Vector2D(temp->v[1].u, temp->v[1].v);
					uv[1] = Vector2D(temp->v[2].u, temp->v[2].v);
					uv[2] = Vector2D(temp->v[0].u, temp->v[0].v);

					t = ((p  - v[2]) * node->normal) / ((v[1] - v[2]) * node->normal);
					p1 = v[2] + (v[1] - v[2]) * t;
					splitted_uv1 = SplitUV(&Vector2D(uv[0].x, uv[0].y), &Vector2D(uv[1].x, uv[1].y), t);

					t = ((p  - v[0]) * node->normal) / ((v[1] - v[0]) * node->normal);
					p2 = v[0] + (v[1] - v[0]) * t;
					splitted_uv2 = SplitUV(&Vector2D(uv[0].x, uv[0].y), &Vector2D(uv[2].x, uv[2].y), t);
				}
				else
				{
					//v[0].
					index = 0;

					uv[0] = Vector2D(temp->v[0].u, temp->v[0].v);
					uv[1] = Vector2D(temp->v[1].u, temp->v[1].v);
					uv[2] = Vector2D(temp->v[2].u, temp->v[2].v);

					t = ((p - v[1]) * node->normal) / ((v[0] - v[1]) * node->normal);
					p1 = v[1] + (v[0] - v[1]) * t;
					splitted_uv1 = SplitUV(&Vector2D(uv[0].x, uv[0].y), &Vector2D(uv[1].x, uv[1].y), t);

					t = ((p - v[2]) * node->normal) / ((v[0] - v[2]) * node->normal);
					p2 = v[2] + (v[0] - v[2]) * t;
					splitted_uv2 = SplitUV(&Vector2D(uv[0].x, uv[0].y), &Vector2D(uv[2].x, uv[2].y), t);
				}
			}

			//Then, divide one triangle to three triangles.
			Vector3D tri1[3], tri2[3], tri3[3];

			tri1[0] = v[index];
			tri1[1] = p1;
			tri1[2] = p2;

			if(index == 0)
			{
				tri2[0] = p1;
				tri2[1] = v[1];
				tri2[2] = v[2];

				tri3[0] = p1;
				tri3[1] = v[2];
				tri3[2] = p2;
			}
			else
			{
				if(index == 1)
				{
					tri2[0] = p1;
					tri2[1] = v[2];
					tri2[2] = v[0];

					tri3[0] = p1;
					tri3[1] = v[0];
					tri3[2] = p2;
				}
				else
				{
					tri2[0] = p1;
					tri2[1] = v[0];
					tri2[2] = v[1];

					tri3[0] = p1;
					tri3[1] = v[1];
					tri3[2] = p2;
				}
			}

			//Allocate two new nodes.
			//Insert these three triangles to node chain instead of intersecting one.
			BSP_Node *newNode1 = new BSP_Node(); 
			BSP_Node *newNode2 = new BSP_Node();

			memcpy(newNode1, temp, sizeof(BSP_Node));
			memcpy(newNode2, temp, sizeof(BSP_Node));

			newNode2->LinkTo = temp->LinkTo;
			newNode1->LinkTo = newNode2;
			temp->LinkTo = newNode1;

			newNode1->LinkFrom = temp;
			newNode1->LinkTo = newNode2;
			newNode2->LinkFrom = newNode1;
			newNode2->LinkTo = temp->LinkTo;
			temp->LinkTo = newNode1;

			//First triangle.
			temp->v[0].x = tri1[0].x; temp->v[0].y = tri1[0].y; temp->v[0].z = tri1[0].z;
			temp->v[0].u = uv[0].x; temp->v[0].v = uv[0].y;
			temp->v[1].x = tri1[1].x; temp->v[1].y = tri1[1].y; temp->v[1].z = tri1[1].z;
			temp->v[1].u = splitted_uv1.x; temp->v[1].v = splitted_uv1.y;
			temp->v[2].x = tri1[2].x; temp->v[2].y = tri1[2].y; temp->v[2].z = tri1[2].z;
			temp->v[2].u = splitted_uv2.x; temp->v[2].v = splitted_uv2.y;
			if(var[index] < 0.0f)
				temp->side = 1;
			else
				temp->side = 0;
			temp->splitted = true;

			//Second triangle.
			newNode1->v[0].x = tri2[0].x; newNode1->v[0].y = tri2[0].y; newNode1->v[0].z = tri2[0].z;
			newNode1->v[0].u = splitted_uv1.x; newNode1->v[0].v = splitted_uv1.y;
			newNode1->v[1].x = tri2[1].x; newNode1->v[1].y = tri2[1].y; newNode1->v[1].z = tri2[1].z;
			newNode1->v[1].u = uv[1].x; newNode1->v[1].v = uv[1].y;
			newNode1->v[2].x = tri2[2].x; newNode1->v[2].y = tri2[2].y; newNode1->v[2].z = tri2[2].z;
			newNode1->v[2].u = uv[2].x; newNode1->v[2].v = uv[2].y;
			newNode1->splitted = true;
			if(temp->side == 0)
				newNode1->side = 1;
			else
				newNode1->side = 0;

			//Third triangle.
			newNode2->v[0].x = tri3[0].x; newNode2->v[0].y = tri3[0].y; newNode2->v[0].z = tri3[0].z;
			newNode2->v[0].u = splitted_uv1.x; newNode2->v[0].v = splitted_uv1.y;
			newNode2->v[1].x = tri3[1].x; newNode2->v[1].y = tri3[1].y; newNode2->v[1].z = tri3[1].z;
			newNode2->v[1].u = uv[2].x; newNode2->v[1].v = uv[2].y;
			newNode2->v[2].x = tri3[2].x; newNode2->v[2].y = tri3[2].y; newNode2->v[2].z = tri3[2].z;
			newNode2->v[2].u = splitted_uv2.x; newNode2->v[2].v = splitted_uv2.y;
			newNode2->splitted = true;
			if(temp->side == 0)
				newNode2->side = 1;
			else
				newNode2->side = 0;

		}
	}
}

void BuildBSPTree::BuildBranches(BSP_Node *node)
{
	BSP_Node *temp, *target = NULL, *pos_target = NULL, *neg_target = NULL;

	//Split all other nodes in same level and set them to be children of this node.
	temp = headnode;
	while(temp != NULL)
	{
		if(temp->side == node->side && temp->Parent == node->Parent && temp != node)
		{
			temp->Parent = node;
			UpdateLink(node, temp);
		}

		temp = temp->LinkTo;
	}

	//Find balanced nodes (next splitting plane) in both sides of this node.
	pos_target = FindBalanceNode(0, node);
	neg_target = FindBalanceNode(1, node);

	//If balanced node on "0" side is found, set it to be child of this node.
	if(pos_target != NULL)
	{
		node->Pos_Child = pos_target;
		node->IsPosChecked = true;
	}

	//If balanced node on "1" side is found, set it to be child of this node.
	if(neg_target != NULL)
	{
		node->Neg_Child = neg_target;
		node->IsNegChecked = true;
	}

	//If no childs, then quit this recursive function.
	if(node->Pos_Child == NULL && node->Neg_Child == NULL)
		return;

	//Continue recursive function for "0" side of this node.
	if(node->Pos_Child != NULL)
		BuildBranches(node->Pos_Child);

	//Continue recursive function for "1" side of this node.
	if(node->Neg_Child != NULL)
		BuildBranches(node->Neg_Child);
}

void BuildBSPTree::BuildWholeBSPTree()
{
	BuildRoot();
}

int BuildBSPTree::num_nodes()
{
	int count = 0;

	BSP_Node *temp = headnode;

	while(temp != NULL)
	{
		count++;
		temp = temp->LinkTo;
	}

	return count;
}

Vector2D BuildBSPTree::SplitUV(Vector2D *p_end, Vector2D *p_start, float t)
{
	Vector2D result;
	result = *p_start + ((*p_end - *p_start) * t);

	return result;
}

int BuildBSPTree::NumSplits(BSP_Node *node)
{
	int loop, numSplits = 0;
	float var[3];
	float A, B, C, D;
	Vector3D v[3];
	BSP_Node *temp = NULL;

	A = node->normal.x;
	B = node->normal.y;
	C = node->normal.z;
	D = -(node->v[2].x * node->normal.x + node->v[2].y * node->normal.y + node->v[2].z * node->normal.z);

	temp = headnode;
	while(temp != NULL)
	{
		if(temp != node && temp->side == node->side && temp->Parent == node->Parent)
		{

			v[0] = Vector3D(temp->v[0].x, temp->v[0].y, temp->v[0].z);
			v[1] = Vector3D(temp->v[1].x, temp->v[1].y, temp->v[1].z);
			v[2] = Vector3D(temp->v[2].x, temp->v[2].y, temp->v[2].z);

			for(loop = 0; loop < 3; loop++)
			{
				var[loop] = A * v[loop].x + B * v[loop].y + C * v[loop].z + D;
				if((float)fabs((float)var[loop]) < 1e-4)
					var[loop] = 0.0f;
			}

			//Check for intersection.
			if(!(var[0] >= 0.0f && var[1] >= 0.0f && var[2] >= 0.0f) && !(var[0] <= 0.0f && var[1] <= 0.0f && var[2] <= 0.0f))
				numSplits++;

		}// End if.

		temp = temp->LinkTo;
	}

	return numSplits;
}

void BuildBSPTree::NumTrianglesOnSides(BSP_Node *node, int *pos, int *neg)
{
	int loop;
	float var[3];
	float A, B, C, D;
	Vector3D v[3];
	BSP_Node *temp = NULL;

	*pos = 0;
	*neg = 0;

	A = node->normal.x;
	B = node->normal.y;
	C = node->normal.z;
	D = -(node->v[2].x * node->normal.x + node->v[2].y * node->normal.y + node->v[2].z * node->normal.z);

	temp = headnode;
	while(temp != NULL)
	{
		if(temp != node && temp->side == node->side && temp->Parent == node->Parent)
		{

			v[0] = Vector3D(temp->v[0].x, temp->v[0].y, temp->v[0].z);
			v[1] = Vector3D(temp->v[1].x, temp->v[1].y, temp->v[1].z);
			v[2] = Vector3D(temp->v[2].x, temp->v[2].y, temp->v[2].z);

			for(loop = 0; loop < 3; loop++)
			{
				var[loop] = A * v[loop].x + B * v[loop].y + C * v[loop].z + D;
				if((float)fabs((float)var[loop]) < 1e-4)
					var[loop] = 0.0f;
			}

			//Check for positive side.
			if(var[0] >= 0.0f && var[1] >= 0.0f && var[2] >= 0.0f)
			{
				*pos += 1;
			}
			else
			{
				//Check for negative side.
				if(var[0] <= 0.0f && var[1] <= 0.0f && var[2] <= 0.0f)
					*neg += 1;
				else
				{
					//Triangle is intersecting with plane.

					int count = 0;

					if(var[0] >= 0.0f)
						count++;
					if(var[1] >= 0.0f)
						count++;
					if(var[2] >= 0.0f)
						count++;

					if(count == 1)
					{
						*pos += 1;
						*neg += 2;
					}
					else
					{
						*pos += 2;
						*neg += 1;
					}
				}
			}
			// End check sides.
		}// End if.

		temp = temp->LinkTo;
	}
}

BSP_Node * BuildBSPTree::FindBalanceNode(bool side, BSP_Node *node)
{
	BSP_Node *temp = NULL, *target = NULL;
	int pos = 0, neg = 0;
	int diff, prev_diff = -1;
	int smallest = 0;

	temp = headnode;
	while(temp != NULL)
	{
		if(temp->side == side && temp->Parent == node && temp != node)
		{
			pos = 0;
			neg = 0;
			NumTrianglesOnSides(temp, &pos, &neg);

			//if(pos > 0 || neg > 0)
			{
				diff = pos - neg;
				if(diff < 0)
					diff = -diff;
				
				if(prev_diff == -1)
				{
					target = temp;
					smallest = diff;
				}
				else
				{
					if(diff < prev_diff)
					{
						target = temp;
						smallest = diff;
					}

				}
				prev_diff = diff;
			}
			//else
				//target = temp;
		}

		temp = temp->LinkTo;
	}

	return target;
}

BSP_Node * BuildBSPTree::FindMinSplits(bool side, BSP_Node *node)
{
	BSP_Node *temp = NULL, *target = NULL;
	int numSplits = 0, prev_numSplits = -1;
	int smallest = 0;

	temp = headnode;
	while(temp != NULL)
	{
		if(temp->side == side && temp->Parent == node && temp != node)
		{
			numSplits = NumSplits(temp);

			if(prev_numSplits == -1)
			{
				target = temp;
				smallest = numSplits;
			}
			else
			{
				if(numSplits < prev_numSplits)
				{
					target = temp;
					smallest = numSplits;
				}

			}
			prev_numSplits = numSplits;

		}

		temp = temp->LinkTo;
	}

	return target;
}
