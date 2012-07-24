#ifndef TEST_MATRIX_H
#define TEST_MATRIX_H

#include "ttest.h"
#include "mmatrix.h"

class TestMMatrix: public TTestCheck
{
public:

	TestMMatrix(): TTestCheck("MMatrix check") {}

	bool MatrixCompare(MMatrix4x4& refMat, MMatrix4x4& ourMat)
	{
		for (int i=0;i<16;i++)
		{
			float diff = refMat.n[i] - ourMat.n[i];
			if (MathDriver::Absolute(diff) > 0.0001)
			{
				TStringBuilderStack<512> sb;

				sb.AppendLine("These matrices is not equal");

				sb.AppendLine("Reference Matrix:");
				refMat.ToStringBuilder(sb);

				sb.AppendLine();
				sb.AppendLine("Our Matrix:");
				ourMat.ToStringBuilder(sb);

				AddOutput(sb.ToString());

				return false;
			}
		}

		return true;
	}

	void Test()
	{
		/// REFERENCE MATRICES WITH CORRECT RESULTS
		MMatrix4x4 PerspectiveFovR ( 1.357995f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 2.414213f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -1.001001f, -1.000000f, 0.000000f, 0.000000f, -0.100100f, 0.000000f );
		MMatrix4x4 PerspectiveFovL ( 1.357995f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 2.414213f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.001001f, 1.000000f, 0.000000f, 0.000000f, -0.100100f, 0.000000f );
		MMatrix4x4 OrthoL ( 0.001563f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.002778f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, -0.000000f, 1.000000f );
		MMatrix4x4 OrthoR ( 0.001563f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.002778f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -1.000000f, 0.000000f, 0.000000f, 0.000000f, -0.000000f, 1.000000f );
		MMatrix4x4 OrthoOffCenterR ( 0.001563f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -0.002778f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -1.000000f, 0.000000f, -1.000000f, 1.000000f, -0.000000f, 1.000000f );
		MMatrix4x4 OrthoOffCenterL ( 0.001563f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -0.002778f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, -1.000000f, 1.000000f, -0.000000f, 1.000000f );
		MMatrix4x4 PerspectiveL ( 0.000156f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000278f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.001001f, 1.000000f, 0.000000f, 0.000000f, -0.100100f, 0.000000f );
		MMatrix4x4 PerspectiveR ( 0.000156f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000278f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -1.001001f, -1.000000f, 0.000000f, 0.000000f, -0.100100f, 0.000000f );
		MMatrix4x4 PerspectiveOffCenterL ( 0.000156f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -0.000278f, 0.000000f, 0.000000f, -1.000000f, 1.000000f, 1.001001f, 1.000000f, 0.000000f, 0.000000f, -0.100100f, 0.000000f );
		MMatrix4x4 PerspectiveOffCenterR ( 0.000156f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -0.000278f, 0.000000f, 0.000000f, 1.000000f, -1.000000f, -1.001001f, -1.000000f, 0.000000f, 0.000000f, -0.100100f, 0.000000f );
		MMatrix4x4 LookAtR ( 0.707107f, -0.408248f, 0.577350f, 0.000000f, 0.000000f, 0.816496f, 0.577350f, 0.000000f, -0.707107f, -0.408248f, 0.577350f, 0.000000f, -0.000000f, -0.000000f, -51.961521f, 1.000000f );
		MMatrix4x4 LookAtL ( -0.707107f, -0.408248f, -0.577350f, 0.000000f, 0.000000f, 0.816496f, -0.577350f, 0.000000f, 0.707107f, -0.408248f, -0.577350f, 0.000000f, -0.000000f, -0.000000f, 51.961521f, 1.000000f );

		MProjectionMatrix persFovR;
		persFovR.PerpectiveFovR(DegreeAngle(45.0f),1280.0f / 720.0f, 0.1f, 100.0f);
		AddResult( MatrixCompare(PerspectiveFovR, persFovR), "Right Handed Perspective FOV Projection Matrix");

		MProjectionMatrix persFovL;
		persFovL.PerpectiveFovL(DegreeAngle(45.0f),1280.0f / 720.0f, 0.1f, 100.0f);
		AddResult( MatrixCompare(PerspectiveFovL, persFovL), "Left Handed Perspective FOV Projection Matrix");

		MProjectionMatrix ortL;
		ortL.OrthoL(1280.0f,720.0f,0.0f, 1.0f);
		AddResult( MatrixCompare(OrthoL, ortL), "Left Handed Orthographic Projection Matrix");

		MProjectionMatrix ortR;
		ortR.OrthoR(1280.0f,720.0f,0.0f, 1.0f);
		AddResult( MatrixCompare(OrthoR, ortR), "Right Handed Orthographic Projection Matrix");

		MProjectionMatrix ortOffR;
		ortOffR.OrthoOffCenterR(0.0f,1280.0f,720.0f,0.0f,0.0f,1.0f);
		AddResult( MatrixCompare(OrthoOffCenterR, ortOffR), "Right Handed Orthographic Off Center Projection Matrix");

		MProjectionMatrix ortOffL;
		ortOffL.OrthoOffCenterL(0.0f,1280.0f,720.0f,0.0f,0.0f,1.0f);
		AddResult( MatrixCompare(OrthoOffCenterL, ortOffL), "Left Handed Orthographic Off Center Projection Matrix");

		MProjectionMatrix persL;
		persL.PerspectiveL(1280.0f,720.0f,0.1f, 100.0f);
		AddResult( MatrixCompare(PerspectiveL, persL), "Left Handed Perspective Projection Matrix");

		MProjectionMatrix persR;
		persR.PerspectiveR(1280.0f,720.0f,0.1f, 100.0f);
		AddResult( MatrixCompare(PerspectiveR, persR), "Right Handed Perspective Projection Matrix");

		MProjectionMatrix persOffL;
		persOffL.PerspectiveOffCenterL(0.0f, 1280.0f, 720.0f, 0.0f,0.1f, 100.0f);
		AddResult( MatrixCompare(PerspectiveOffCenterL, persOffL), "Left Handed Perspective Off Center Projection Matrix");

		MProjectionMatrix persOffR;
		persOffR.PerspectiveOffCenterR(0.0f, 1280.0f, 720.0f, 0.0f,0.1f, 100.0f);
		AddResult( MatrixCompare(PerspectiveOffCenterR, persOffR), "Right Handed Perspective Off Center Projection Matrix");

		MViewMatrix lookR;
		lookR.LookAtR(Vector3(30,30,30),Vector3(0,0,0),Vector3(0,1,0));
		AddResult( MatrixCompare(LookAtR, lookR), "Right Handed Look At View Matrix");

		MViewMatrix lookL;
		lookL.LookAtL(Vector3(30,30,30),Vector3(0,0,0),Vector3(0,1,0));
		AddResult( MatrixCompare(LookAtL, lookL), "Left Handed Look At View Matrix");
	}

} MMatrixCheck;



#endif