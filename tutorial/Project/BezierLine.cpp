#include "BezierLine.h"

Bezier1D::Bezier1D(void)
{
	numberOfPoints = 4;

	Eigen::Matrix4d Model = Eigen::Matrix4d();
	Model << 0, 0.0, 0.0, 0.0,
			1.5-0.5, 1.0, 0.0, 0.0,
			1.5+0.5, 1.0, 0.0, 0.0,
			1.5 +1.5, -1.0, 0.0, 0.0;
	segments.push_back(Model);

}


IndexedModel Bezier1D::GetLine(int resT) {

	std::vector<LineVertex> axisVertices;
	Eigen::Matrix4d sector = segments[0];
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
	Eigen::Vector3d* CP3 = GetControlPoint(segment, 0).GetPos();
	Eigen::Vector3d* CP2 = GetControlPoint(segment, 1).GetPos();
	Eigen::Vector3d* CP1 = GetControlPoint(segment, 2).GetPos();
	Eigen::Vector3d* CP0 = GetControlPoint(segment, 3).GetPos();
	Eigen::Vector3d thevector;
	
	thevector <<
		(CP0->x() * t * t * t) + (CP1->x() * 3 * t * t * (1 - t)) + (CP2->x() * 3 * t * (1 - t) * (1 - t)) + (CP3->x() * (1 - t) * (1 - t) * (1 - t)), // point x
		(CP0->y() * t * t * t) + (CP1->y() * 3 * t * t * (1 - t)) + (CP2->y() * 3 * t * (1 - t) * (1 - t)) + (CP3->y() * (1 - t) * (1 - t) * (1 - t)), //point y
		(CP0->z() * t * t * t) + (CP2->z() * 3 * t * t * (1 - t)) + (CP2->z() * 3 * t * (1 - t) * (1 - t)) + (CP3->z() * (1 - t) * (1 - t) * (1 - t))  // point z
		;
	
	
	Eigen::Vector3f tempcolor; tempcolor << 0, 0, 1;
	LineVertex output(thevector, tempcolor);
	return output;

}



//note change to 3d?
void Bezier1D::MoveControlPoint(int segment, int index, bool preserveC1, Eigen::Vector4d newPosition) {

	if (segment == segments.size()) { // and..
		segment = segment - 1;
		index = 3;
	}

	Eigen::Matrix4d sector = segments[segment];
	Eigen::Vector4d oldpos = sector.row(index);

	//Eigen::Matrix4d sector = segments[segment];
	//Eigen::Vector4d oldpos;oldpos<< sector(index, 0), sector(index, 1), sector(index, 2), 0;
	Eigen::Vector4d diff; diff = newPosition - oldpos;


	
	segments[segment].row(index) = newPosition;

	if (index == 0 && segment != 0) // we need to move the copy of the point on the previos seg
		(segments[segment - 1]).row(3) = newPosition;
	if (preserveC1)
	{
		if (segment == 0 && index == 0) {
			segments[segment].row(1) += diff; // segments[0][1] += diff;
			return;
		}
		switch (index)
		{
		case 0:
			segments[segment - 1].row(2) += diff;
			segments[segment].row(1) += diff;
			break;
		case 1:
			if (segment != 0)
				segments[segment - 1].row(2) -= diff;
			break;
		case 2:
			if (segment != segments.size() - 1)
				segments[segment + 1].row(1) -= diff;
			break;
		case 3: // last point on line
			segments[segment].row(2) += diff;
		}

	}
	
}



Eigen::Vector3d Bezier1D::GetVelosity(int segment, float t) {
	Eigen::Vector3d* CP3 = GetControlPoint(segment, 0).GetPos();
	Eigen::Vector3d* CP2 = GetControlPoint(segment, 1).GetPos();
	Eigen::Vector3d* CP1 = GetControlPoint(segment, 2).GetPos();
	Eigen::Vector3d* CP0 = GetControlPoint(segment, 3).GetPos();

	float tp0 = -3 * pow((1 - t), 2);
	float tp1 = 3 * (1 - t) * (1 - t) - 6 * t * (1 - t);	//-3 * (-3 * t*t + 4 * t - 1);
	float tp2 = 6 * t * (1 - t) - 3 * t * t; // -3 * (3 * pow(t, 2) - 2 * t);
	float tp3 = 3 * pow(t, 2);
	return (*CP3 * tp3 + *CP2 * tp2 + *CP1 * tp1 + *CP0 * tp0);

}




LineVertex Bezier1D::GetControlPoint(int seg, int index)
{
	if (seg == segments.size()) {
		seg--;
		index = 3;
	}
	Eigen::Matrix4d sector = segments[seg];
	Eigen::Vector3d thepoint0; // <- sector[index]
	//thepoint0 << sector(index,0), sector(index,1),sector(index,2);
	thepoint0 = sector.row(index).head<3>();
	


	Eigen::Vector3f tempcolor; tempcolor << 0, 0, 1;
	LineVertex output(thepoint0, tempcolor);
	return output;
}