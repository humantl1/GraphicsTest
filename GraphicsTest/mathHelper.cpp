

inline static Eigen::RowVector2f vectorTranslate(Eigen::RowVector2f point, Eigen::Matrix2f basis)
{
	point *= basis;

	return point;
}

inline static Eigen::RowVector2f lineTranslate(
	Eigen::RowVector2f p1, 
	Eigen::RowVector2f p2,
	Eigen::Matrix2f basis)
{
	p1 *= basis;
	p2 *= basis;

	return point;
}

inline static float deg_to_rad(float degrees)
{
	return degrees * 3.14159 / 180;
}