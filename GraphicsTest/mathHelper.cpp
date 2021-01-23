

inline static Eigen::RowVector2i intVectorTranslate(Eigen::RowVector2i v, Eigen::Matrix2f basis)
{


	v *= basis;

	return v;
}

inline static float deg_to_rad(float degrees)
{
	return degrees * 3.14159 / 180;
}