//class Vertice
//{
//public:
//
//	int x, y;
//	Eigen::Vector2i point;
//
//	Vertice()
//	{
//		x = 0;
//		y = 0;
//		point(0) = 0;
//		point(1) = 0;
//	}
//
//	Vertice(int xIn, int yIn)
//	{
//		x = xIn;
//		y = yIn;
//
//		point(0) = x;
//		point(1) = y;
//	}
//
//	Vertice(Eigen::Vector2i pointIn)
//	{
//		x = pointIn(0);
//		y = pointIn(1);
//
//		point = pointIn;
//	}
//};
//
//class Line
//{
//public:
//
//	Vertice v0;
//	Vertice v1;
//
//	Line()
//	{
//		
//	}
//
//	Line(Vertice v0In, Vertice v1In)
//	{
//		v0 = v0In;
//		v1 = v1In;
//	}
//
//	Line(int x0, int y0, int x1, int y1)
//	{
//		v0.x = x0;
//		v0.y = y0;
//		v1.x = x1;
//		v1.y = y1;
//	}
//
//	Line(Eigen::Vector2i p0, Eigen::Vector2i p1)
//	{
//		v0.x = p0(0);
//		v0.y = p0(1);
//		v1.x = p1(0);
//		v1.y = p1(1);
//	}
//
//	Line lineTranslate(Line l, Eigen::Matrix2f basis)
//	{
//		l.v0.point = l.v0.point * basis;
//		l.v1.point = l.v1.point * basis;
//
//		return l;
//	}
//};
//
