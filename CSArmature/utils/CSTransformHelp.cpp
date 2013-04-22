/*
 * Copyright (c) 2012 Chukong Technologies, Inc.
 *
 * http://www.cocostudio.com
 * http://tools.cocoachina.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
 * NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "CSTransformHelp.h"
#include "CSUtilMath.h"

namespace cs {

CCAffineTransform TransformHelp::helpMatrix1;
CCAffineTransform TransformHelp::helpMatrix2;

CCPoint TransformHelp::helpPoint1;
CCPoint TransformHelp::helpPoint2;

Node helpParentNode;

TransformHelp::TransformHelp()
{
}

void TransformHelp::transformFromParent(Node &node, const Node &parentNode)
{
	nodeToMatrix(node, helpMatrix1);
	nodeToMatrix(parentNode, helpMatrix2);

	helpMatrix2 = CCAffineTransformInvert(helpMatrix2);
	helpMatrix1 = CCAffineTransformConcat(helpMatrix1, helpMatrix2);

	matrixToNode(helpMatrix1, node);
}

void TransformHelp::transformToParent(Node &node, const Node &parentNode)
{

	nodeToMatrix(node, helpMatrix1);
	nodeToMatrix(parentNode, helpMatrix2);

	helpMatrix1 = CCAffineTransformConcat(helpMatrix1, helpMatrix2);

	matrixToNode(helpMatrix1, node);
}

void TransformHelp::transformFromParentWithoutScale(Node &node, const Node &parentNode)
{

	helpParentNode.copy(&parentNode);
	helpParentNode.m_fScaleX = 1;
	helpParentNode.m_fScaleY = 1;

	nodeToMatrix(node, helpMatrix1);
	nodeToMatrix(helpParentNode, helpMatrix2);

	helpMatrix2 = CCAffineTransformInvert(helpMatrix2);
	helpMatrix1 = CCAffineTransformConcat(helpMatrix1, helpMatrix2);

	matrixToNode(helpMatrix1, node);
}

void TransformHelp::transformToParentWithoutScale(Node &node, const Node &parentNode)
{

	helpParentNode.copy(&parentNode);
	helpParentNode.m_fScaleX = 1;
	helpParentNode.m_fScaleY = 1;

	nodeToMatrix(node, helpMatrix1);
	nodeToMatrix(helpParentNode, helpMatrix2);

	helpMatrix1 = CCAffineTransformConcat(helpMatrix1, helpMatrix2);

	matrixToNode(helpMatrix1, node);
}

void TransformHelp::nodeToMatrix(const Node &node, CCAffineTransform &matrix)
{
	matrix.a = node.m_fScaleX * cos(node.m_fSkewY);
	matrix.b = node.m_fScaleX * sin(node.m_fSkewY);
	matrix.c = node.m_fScaleY * sin(node.m_fSkewX);
	matrix.d = node.m_fScaleY * cos(node.m_fSkewX);

	matrix.tx = node.m_fX;
	matrix.ty = node.m_fY;
}

void TransformHelp::matrixToNode(const CCAffineTransform &matrix, Node &node)
{
    /*
     *  In as3 language, there is a function called "deltaTransformPoint", it calculate a point used give Transform
     *  but not used the tx, ty value. we simulate the function here
     */
	helpPoint1.x = 0;
	helpPoint1.y = 1;
	helpPoint1 = CCPointApplyAffineTransform(helpPoint1, matrix);
	helpPoint1.x -= matrix.tx;
	helpPoint1.y -= matrix.ty;

	helpPoint2.x = 1;
	helpPoint2.y = 0;
	helpPoint2 = CCPointApplyAffineTransform(helpPoint2, matrix);
	helpPoint2.x -= matrix.tx;
	helpPoint2.y -= matrix.ty;



	node.m_fSkewX = -(atan2f(helpPoint1.y, helpPoint1.x) - 1.5707964f);
	node.m_fSkewY = atan2f(helpPoint2.y, helpPoint2.x);
	node.m_fScaleX = sqrt(matrix.a * matrix.a + matrix.b * matrix.b);
	node.m_fScaleY = sqrt(matrix.c * matrix.c + matrix.d * matrix.d);
	node.m_fX = matrix.tx;
	node.m_fY = matrix.ty;
}

void TransformHelp::nodeConcat(Node &target, Node &source)
{
	target.m_fX += source.m_fX;
	target.m_fY += source.m_fY;
	target.m_fSkewX += source.m_fSkewX;
	target.m_fSkewY += source.m_fSkewY;
	target.m_fScaleX += source.m_fScaleX;
	target.m_fScaleY += source.m_fScaleY;
}

}