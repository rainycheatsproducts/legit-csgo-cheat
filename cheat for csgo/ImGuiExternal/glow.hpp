#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class Color
{
public:
	Color ( );
	Color ( int _r, int _g, int _b );
	Color ( int _r, int _g, int _b, int _a );
	Color ( float _r, float _g, float _b ) : Color ( _r, _g, _b, 1.0f ) {}
	Color ( float _r, float _g, float _b, float _a )
		: Color (
			static_cast< int >( _r * 255.0f ),
			static_cast< int >( _g * 255.0f ),
			static_cast< int >( _b * 255.0f ),
			static_cast< int >( _a * 255.0f ) )
	{
	}
	explicit Color ( float* rgb ) : Color ( rgb[ 0 ], rgb[ 1 ], rgb[ 2 ], 1.0f ) {}
	explicit Color ( unsigned long argb )
	{
		_CColor[ 2 ] = ( unsigned char )( ( argb & 0x000000FF ) >> ( 0 * 8 ) );
		_CColor[ 1 ] = ( unsigned char )( ( argb & 0x0000FF00 ) >> ( 1 * 8 ) );
		_CColor[ 0 ] = ( unsigned char )( ( argb & 0x00FF0000 ) >> ( 2 * 8 ) );
		_CColor[ 3 ] = ( unsigned char )( ( argb & 0xFF000000 ) >> ( 3 * 8 ) );
	}

	void    SetRawColor ( int color32 );
	int     GetRawColor ( ) const;
	void    SetColor ( int _r, int _g, int _b, int _a = 0 );
	void    SetColor ( float _r, float _g, float _b, float _a = 0 );
	void    GetColor ( int& _r, int& _g, int& _b, int& _a ) const;

	std::string GetNormalnijHexColor ( ) const;

	int r ( ) const { return _CColor[ 0 ]; }
	int g ( ) const { return _CColor[ 1 ]; }
	int b ( ) const { return _CColor[ 2 ]; }
	int a ( ) const { return _CColor[ 3 ]; }

	int& operator[]( int index )
	{
		return _CColor[ index ];
	}
	const int& operator[]( int index ) const
	{
		return _CColor[ index ];
	}

	bool operator==( const Color& rhs ) const;
	bool operator!=( const Color& rhs ) const;
	Color& operator=( const Color& rhs );

	static Color FromHSB ( float hue, float saturation, float brightness )
	{
		float h = hue == 1.0f ? 0 : hue * 6.0f;
		float f = h - ( int )h;
		float p = brightness * ( 1.0f - saturation );
		float q = brightness * ( 1.0f - saturation * f );
		float t = brightness * ( 1.0f - ( saturation * ( 1.0f - f ) ) );

		if ( h < 1 )
		{
			return Color (
				( unsigned char )( brightness * 255 ),
				( unsigned char )( t * 255 ),
				( unsigned char )( p * 255 )
			);
		}
		else if ( h < 2 )
		{
			return Color (
				( unsigned char )( q * 255 ),
				( unsigned char )( brightness * 255 ),
				( unsigned char )( p * 255 )
			);
		}
		else if ( h < 3 )
		{
			return Color (
				( unsigned char )( p * 255 ),
				( unsigned char )( brightness * 255 ),
				( unsigned char )( t * 255 )
			);
		}
		else if ( h < 4 )
		{
			return Color (
				( unsigned char )( p * 255 ),
				( unsigned char )( q * 255 ),
				( unsigned char )( brightness * 255 )
			);
		}
		else if ( h < 5 )
		{
			return Color (
				( unsigned char )( t * 255 ),
				( unsigned char )( p * 255 ),
				( unsigned char )( brightness * 255 )
			);
		}
		else
		{
			return Color (
				( unsigned char )( brightness * 255 ),
				( unsigned char )( p * 255 ),
				( unsigned char )( q * 255 )
			);
		}
	}

	static Color Black;
	static Color White;
	static Color Red;
	static Color Green;
	static Color Blue;

	int _CColor[ 4 ];
};

class c_color
{
public:
	float r, g, b, a;

	c_color ( float _r, float _g, float _b, float _a )
	{

		r = _r;
		g = _g;
		b = _b;
		a = _a;

	}

	c_color ( uint32_t color )
	{
		this->a = ( color >> 24 ) & 0xff;
		this->r = ( color >> 16 ) & 0xff;
		this->g = ( color >> 8 ) & 0xff;
		this->b = ( color & 0xff );
	}

	static c_color from_hsb ( float flHue, float flSaturation, float flBrightness )
	{
		const float h = std::fmodf ( flHue, 1.0f ) / ( 60.0f / 360.0f );
		const int i = static_cast< int >( h );
		const float f = h - static_cast< float >( i );
		const float p = flBrightness * ( 1.0f - flSaturation );
		const float q = flBrightness * ( 1.0f - flSaturation * f );
		const float t = flBrightness * ( 1.0f - flSaturation * ( 1.0f - f ) );

		float r = 0.0f, g = 0.0f, b = 0.0f;

		switch ( i )
		{
		case 0:
			r = flBrightness, g = t, b = p;
			break;
		case 1:
			r = q, g = flBrightness, b = p;
			break;
		case 2:
			r = p, g = flBrightness, b = t;
			break;
		case 3:
			r = p, g = q, b = flBrightness;
			break;
		case 4:
			r = t, g = p, b = flBrightness;
			break;
		case 5:
		default:
			r = flBrightness, g = p, b = q;
			break;
		}

		return c_color ( r, g, b, 255 );
	}

	void random_color ( int iTick )
	{
		this->r = sin ( 3.f * iTick + 0.f ) * 127 + 128;
	}
};