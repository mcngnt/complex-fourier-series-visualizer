#include <complex>
#include "vector.h"

#define INT_DT 0.0001
#define EVAPORATION_SPEED 0.0008
#define CYCLE_DURATION 700

struct instruction
{
	char type;
	std::vector<float> positions;
};

struct coeficient
{
	std::complex<float> val;
	int index;
};

struct equation
{
	std::complex<float> a;
	std::complex<float> b;
	std::complex<float> c;
	std::complex<float> d;
};


class CFS
{
public:
	void generate_target_eq(std::string fileName);
	std::complex<float> apply_target_equation(float t);
	std::complex<float> apply_cfs_equation(float t);
	std::complex<float> calculate_nth_term(int n);
	std::complex<float> apply_cfs_equation_at_n(float t, int n);
	void calculate_coef();
	void sort_coef();
	Vector img_to_world_pos(Vector imgV);
	Vector world_to_img_pos(Vector worldV);
	Vector world_to_screen_pos(Vector worlV);

	std::vector<equation> targetEq;
	int eqNB = 0;
	Vector svgSize = vec_v(0,0);
	int circlesNB = 70;
	std::vector<coeficient> coef;
};