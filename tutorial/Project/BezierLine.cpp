#include "BezierLine.h"

Bezier1D::Bezier1D(void)
{
	numberOfPoints = 4;

	Eigen::Matrix4f Model = Eigen::Matrix4f();
	Model << 0 - 1.5, 0.0, 0.0, 0.0,
			1 - 1.5, 1.0, 0.0, 0.0,
			2 - 1.5, 1.0, 0.0, 0.0,
			3 - 1.5, 1.0, 0.0, 0.0;
	segments.push_back(Model);

}


IndexedModel Bezier1D::GetLine(int resT) {

	std::vector<LineVertex> axisVertices;
	Eigen::Matrix4f sector = segments[0];
	for (int s = 0; s < segments.size(); s++)
	{
		for (int i = 0; i < resT; i++)
		{
			double t = (double)i / (double)resT;
			axisVertices.push_back(GetVertex(s, t));
		}
	}
	axisVertices.push_back(GetVertex(segments.size() - 1, 1));

	std::vector<unsigned int> axisIndices;

	for (int i = 0; i < axisVertices.size() - 1; i++)
	{
		axisIndices.push_back(i);
		axisIndices.push_back(i + 1);
	}
	axisIndices.push_back(axisVertices.size());

	IndexedModel model;
	for (unsigned int i = 0; i < axisVertices.size(); i++)
	{
		model.positions.push_back(*axisVertices[i].GetPos());
		model.colors.push_back(*axisVertices[i].GetColor());

	}
	for (unsigned int i = 0; i < axisIndices.size(); i++)
		model.indices.push_back(axisIndices[i]);

	return model;

}

LineVertex Bezier1D::GetVertex(int segment, float t) {
	Eigen::Vector3f* CP3 = GetControlPoint(segment, 0).GetPos();
	Eigen::Vector3f* CP2 = GetControlPoint(segment, 1).GetPos();
	Eigen::Vector3f* CP1 = GetControlPoint(segment, 2).GetPos();
	Eigen::Vector3f* CP0 = GetControlPoint(segment, 3).GetPos();

	Eigen::Vector3f thevector;
	//float a = cp3->x();
	//float temp = thevector;
	/*
	glm::vec3 myvertex(
		(CP0.GetPos()->x * t * t * t) + (CP1.GetPos()->x * 3 * t * t * (1 - t)) + (CP2.GetPos()->x * 3 * t * (1 - t) * (1 - t)) + (CP3.GetPos()->x * (1 - t) * (1 - t) * (1 - t)), // point x
		(CP0.GetPos()->y * t * t * t) + (CP1.GetPos()->y * 3 * t * t * (1 - t)) + (CP2.GetPos()->y * 3 * t * (1 - t) * (1 - t)) + (CP3.GetPos()->y * (1 - t) * (1 - t) * (1 - t)), //point y
		(CP0.GetPos()->z * t * t * t) + (CP2.GetPos()->z * 3 * t * t * (1 - t)) + (CP2.GetPos()->z * 3 * t * (1 - t) * (1 - t)) + (CP3.GetPos()->z * (1 - t) * (1 - t) * (1 - t))  // point z
	);
	*/

	
	
	thevector <<
		(CP0->x() * t * t * t) + (CP1->x() * 3 * t * t * (1 - t)) + (CP2->x() * 3 * t * (1 - t) * (1 - t)) + (CP3->x() * (1 - t) * (1 - t) * (1 - t)), // point x
		(CP0->y() * t * t * t) + (CP1->y() * 3 * t * t * (1 - t)) + (CP2->y() * 3 * t * (1 - t) * (1 - t)) + (CP3->y() * (1 - t) * (1 - t) * (1 - t)), //point y
		(CP0->z() * t * t * t) + (CP2->z() * 3 * t * t * (1 - t)) + (CP2->z() * 3 * t * (1 - t) * (1 - t)) + (CP3->z() * (1 - t) * (1 - t) * (1 - t))  // point z
		;
	

	Eigen::Vector3f tempcolor; tempcolor << 0, 0, 1;
	LineVertex output(thevector, tempcolor);
	return output;

}



/*
void Bezier1D::MoveControlPoint(int segment, int index, bool preserveC1, Eigen::Vector4f newPosition) {

	if (segment == segments.size()) { // and..
		segment = segment - 1;
		index = 3;
	}
	Eigen::Matrix4f sector = segments[segment];
	Eigen::Vector4f oldpos;oldpos<< sector(index, 0), sector(index, 1), sector(index, 2), 0;
	Eigen::Vector4f diff; diff = newPosition - oldpos;



	segments[segment][index] = newPosition;

	if (indx == 0 && segment != 0) // we need to move the copy of the point on the previos seg
		(segments[segment - 1])[3] = newPosition;
	if (preserveC1)
	{
		if (segment == 0 && indx == 0) {
			segments[segment][1] += diff; // segments[0][1] += diff;
			return;
		}
		switch (indx)
		{
		case 0:
			segments[segment - 1][2] += diff;
			segments[segment][1] += diff;
			break;
		case 1:
			if (segment != 0)
				segments[segment - 1][2] -= diff;
			break;
		case 2:
			if (segment != segments.size() - 1)
				segments[segment + 1][1] -= diff;
			break;
		case 3: // last point on line
			segments[segment][2] += diff;
		}

	}

}
*/

/*
glm::vec3  Bezier1D::GetVelosity(int segment, float t) {
	glm::vec3 CP3 = *GetControlPoint(segment, 3).GetPos();
	glm::vec3 CP2 = *GetControlPoint(segment, 2).GetPos();
	glm::vec3 CP1 = *GetControlPoint(segment, 1).GetPos();
	glm::vec3 CP0 = *GetControlPoint(segment, 0).GetPos();
	//CP3.z = 0;
	//CP2.z = 0;
	//CP1.z = 0;
	//CP0.z = 0;
	float tp0 = -3 * pow((1 - t), 2);
	float tp1 = 3 * (1 - t) * (1 - t) - 6 * t * (1 - t);	//-3 * (-3 * t*t + 4 * t - 1);
	float tp2 = 6 * t * (1 - t) - 3 * t * t; // -3 * (3 * pow(t, 2) - 2 * t);
	float tp3 = 3 * pow(t, 2);

	return (CP3 * tp3 + CP2 * tp2 + CP1 * tp1 + CP0 * tp0);
	//	glm::vec3 myvertex(
	//	(CP0.GetPos()->x * 3 * t*t) + (CP1.GetPos()->x * 6 * t*t*(1 - t)) + (CP2.GetPos()->x * 3 * t*(1 - t)*(1 - t)) + (CP3.GetPos()->x * (1 - t)*(1 - t)*(1 - t)), // point x
	//	(CP0.GetPos()->y * t*t*t) + (CP1.GetPos()->y * 3 * t*t*(1 - t)) + (CP2.GetPos()->y * 3 * t*(1 - t)*(1 - t)) + (CP3.GetPos()->y * (1 - t)*(1 - t)*(1 - t)), //point y
	//	(CP0.GetPos()->z * t*t*t) + (CP2.GetPos()->z * 3 * t*t*(1 - t)) + (CP2.GetPos()->z * 3 * t*(1 - t)*(1 - t)) + (CP3.GetPos()->z * (1 - t)*(1 - t)*(1 - t))  // point z
	//);

	//return myvertex;
}


Bezier1D::~Bezier1D(void)
{
}
*/

LineVertex Bezier1D::GetControlPoint(int seg, int index)
{
	if (seg == segments.size()) {
		seg--;
		index = 3;
	}
	Eigen::Matrix4f sector = segments[seg];
	Eigen::Vector3f thepoint0; // <- sector[index]
	thepoint0 << sector(index,0), sector(index,1),sector(index,2);
	Eigen::Vector3f tempcolor; tempcolor << 0, 0, 1;
	LineVertex output(thepoint0, tempcolor);
	return output;
}