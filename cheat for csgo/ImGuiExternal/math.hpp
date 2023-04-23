#pragma once
#include <math.h>
#include <algorithm>
#define M_PI 3.141592653589793238462643383279502884197169399375105820974944f
#define M_PI_2 1.57079632679489661923f
#define M_PI_F ((float)(M_PI))
#define RAD2DEG(x) ((float)(x) * (float)(180.f / M_PI_F))
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define powFFFFFFFFFFFFFFFFFFFFFF(n) (n)*(n)
#define min(x,y) ((x)<(y) ? x:y)
#define wh(a) ImColor(1.f,1.f,1.f,a)

namespace geo
{
	class vec2_t
	{
	public:
		float x, y;

		vec2_t ( ) { x = y = 0.f; }

		vec2_t ( float x, float y ) { this->x = x; this->y = y; }

		auto operator-( const geo::vec2_t& v ) -> geo::vec2_t { return geo::vec2_t ( x - v.x, y - v.y ); }

		auto is_empty ( ) -> bool
		{
			if ( this->x == 0 || this->y == 0 )
				return true;
			else
				return false;
		}

		auto dot ( geo::vec2_t input ) -> float
		{
			return ( x * input.x ) + ( y * input.y );
		}

		auto length_sqr ( ) -> float
		{
			return ( x * x ) + ( y * y );
		}

		auto length ( ) -> float
		{
			return ( float )sqrt ( length_sqr ( ) );
		}

		auto distance ( geo::vec2_t input ) -> float
		{
			return ( *this - input ).length ( );
		}

		auto clear ( ) -> void
		{
			this->x = 0.f;
			this->y = 0.f;
		}
	};

	class vec3_t
	{
	public:
		float x, y, z;

		vec3_t ( ) { x = y = z = 0.f; }

		vec3_t ( float x, float y, float z ) { this->x = x; this->y = y; this->z = z; }

		auto operator+( const geo::vec3_t& input ) -> geo::vec3_t { return geo::vec3_t{ x + input.x, y + input.y, z + input.z }; }

		auto operator-( geo::vec3_t& input ) -> geo::vec3_t { return geo::vec3_t{ x - input.x, y - input.y, z - input.z }; }

		auto operator==( const geo::vec3_t& input ) -> bool { return x == input.x && y == input.y && z == input.z; }

		auto operator-=( const geo::vec3_t& v ) -> geo::vec3_t { x -= v.x; y -= v.y; z -= v.z; return *this; }

		auto operator+=( const geo::vec3_t& v ) -> geo::vec3_t { x += v.x; y += v.y; z += v.z; return *this; }

		auto operator/( float input ) -> geo::vec3_t { return geo::vec3_t{ x / input, y / input, z / input }; }

		auto operator*( float input ) -> geo::vec3_t { return geo::vec3_t{ x * input, y * input, z * input }; }

		auto operator/=( float input ) -> geo::vec3_t { x /= input; y /= input; z /= input; return *this; }

		auto operator*=( float input ) -> geo::vec3_t { x *= input; y *= input; z *= input; return *this; }

		auto is_empty ( ) -> bool
		{
			if ( this->x == 0 || this->y == 0 || this->z == 0 )
				return true;
			else
				return false;
		}

		auto dot ( geo::vec3_t input ) -> float
		{
			return ( x * input.x ) + ( y * input.y ) + ( z * input.z );
		}

		auto length_sqr ( ) -> float
		{
			return ( x * x ) + ( y * y ) + ( z * z );
		}

		auto length ( ) -> float
		{
			return ( float )sqrt ( length_sqr ( ) );
		}

		auto distance ( geo::vec3_t input ) -> float
		{
			return ( *this - input ).length ( );
		}

		static auto Zero ( ) -> geo::vec3_t
		{
			return geo::vec3_t ( 0.00f, 0.00f, 0.00f );
		}

		static auto get_right ( ) -> geo::vec3_t
		{
			return geo::vec3_t ( 1.00f, 0.00f, 0.00f );
		}

		static auto get_left ( ) -> geo::vec3_t
		{
			return geo::vec3_t ( -1.00f, 0.00f, 0.00f );
		}

		static auto get_forward ( ) -> geo::vec3_t
		{
			return geo::vec3_t ( 0.00f, 0.00f, 1.00f );
		}

		static auto get_backward ( ) -> geo::vec3_t
		{
			return geo::vec3_t ( 0.00f, 0.00f, -1.00f );
		}

		static auto get_up ( ) -> geo::vec3_t
		{
			return geo::vec3_t ( 0.00f, 1.00f, 0.00f );
		}

		static auto get_down ( ) -> geo::vec3_t
		{
			return geo::vec3_t ( 0.00f, -1.00f, 0.00f );
		}

		static auto my_sqrt ( float number ) -> float
		{
			long        i;
			float       x2, y;
			const float threehalfs = 1.5F;

			x2 = number * 0.5F;
			y = number;
			i = *( long* )&y; // floating point bit level hacking [sic]
			i = 0x5f3759df - ( i >> 1 ); // Newton's approximation
			y = *( float* )&i;
			y = y * ( threehalfs - ( x2 * y * y ) ); // 1st iteration
			y = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration

			return 1 / y;
		}

		auto to_angle ( ) -> geo::vec3_t
		{
			return geo::vec3_t
			{
				std::atan2 ( -z, std::hypot ( x, y ) ) * ( 180.0f / M_PI ),
				std::atan2 ( y, x ) * ( 180.0f / M_PI ),
				0.0f
			};
		}

		auto clear ( ) -> void
		{
			this->x = 0.f;
			this->y = 0.f;
			this->z = 0.f;
		}
	};

	class vec4_t
	{
	public:
		float x, y, z, w;

		vec4_t ( ) { x = y = z = w = 0.f; }

		vec4_t ( float x, float y, float z, float w ) { this->x = x; this->y = y; this->z = z; this->w = w; }

		auto is_empty ( ) -> bool
		{
			if ( this->x == 0 || this->y == 0 || this->z == 0 || this->w == 0 )
				return true;
			else
				return false;
		}

		auto dot ( geo::vec4_t input ) -> float
		{
			return ( x * input.x ) + ( y * input.y ) + ( z * input.z ) + ( w * input.w );
		}
	};

	auto calculate_bullet_drop ( float height, float distance, float velocity, float gravity ) -> float
	{
		float pitch = atan2 ( height, distance );
		float bullet_velocity = velocity * cos ( pitch );
		float time = distance / bullet_velocity;

		return ( 0.525f * gravity * time * time ) * 10;
	}

	auto to_rad ( double degree ) -> double
	{
		double pi = 3.14159265359;
		return ( degree * ( pi / 180 ) );
	}

	auto to_quat ( geo::vec3_t Euler ) -> geo::vec4_t
	{
		double heading = to_rad ( Euler.x );
		double attitude = to_rad ( Euler.y );
		double bank = to_rad ( Euler.z );

		double c1 = cos ( heading / 2 );
		double s1 = sin ( heading / 2 );
		double c2 = cos ( attitude / 2 );
		double s2 = sin ( attitude / 2 );
		double c3 = cos ( bank / 2 );
		double s3 = sin ( bank / 2 );
		double c1c2 = c1 * c2;
		double s1s2 = s1 * s2;
		geo::vec4_t quat;
		quat.w = c1c2 * c3 - s1s2 * s3;
		quat.x = c1c2 * s3 + s1s2 * c3;
		quat.y = s1 * c2 * c3 + c1 * s2 * s3;
		quat.z = c1 * s2 * c3 - s1 * c2 * s3;
		return { quat.y, quat.z, ( quat.x * -1 ), quat.w };
	}

	auto quaternion_multi ( const geo::vec3_t point, geo::vec4_t* quat ) -> geo::vec3_t
	{
		float num = quat->x * 2.f;
		float num2 = quat->y * 2.f;
		float num3 = quat->z * 2.f;
		float num4 = quat->x * num;
		float num5 = quat->y * num2;
		float num6 = quat->z * num3;
		float num7 = quat->x * num2;
		float num8 = quat->x * num3;
		float num9 = quat->y * num3;
		float num10 = quat->w * num;
		float num11 = quat->w * num2;
		float num12 = quat->w * num3;
		geo::vec3_t result{};
		result.x = ( 1.f - ( num5 + num6 ) ) * point.x + ( num7 - num12 ) * point.y + ( num8 + num11 ) * point.z;
		result.y = ( num7 + num12 ) * point.x + ( 1.f - ( num4 + num6 ) ) * point.y + ( num9 - num10 ) * point.z;
		result.z = ( num8 - num11 ) * point.x + ( num9 + num10 ) * point.y + ( 1.f - ( num4 + num5 ) ) * point.z;
		return result;
	}
}

struct view_matrix_t 
{
	float* operator[ ]( int index ) {
		return matrix[ index ];
	}

	float matrix[ 4 ][ 4 ];
};