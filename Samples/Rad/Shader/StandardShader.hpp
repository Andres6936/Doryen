// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_STANDARDSHADER_HPP
#define DORYEN_STANDARDSHADER_HPP

#include "Interface/Shader.hpp"

class StandardShader : public Shader
{

public :

	StandardShader(Map& _map) noexcept: Shader(_map)
	{
	};

	void compute() override;

	void calculateShaders() override;

};

#endif //DORYEN_STANDARDSHADER_HPP
