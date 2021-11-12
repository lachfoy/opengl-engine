#pragma once

#include <glad/glad.h>

#include <iostream>

class ShadowMapFBO
{
public:
	ShadowMapFBO()
	{
		FBO = 0;
		shadowMap = 0;
	}

	~ShadowMapFBO()
	{
		glDeleteFramebuffers(1, &FBO);
		glDeleteTextures(1, &shadowMap);
	}

	bool init(unsigned int WindowWidth, unsigned int WindowHeight)
	{
		glGenFramebuffers(1, &FBO);

		glGenTextures(1, &shadowMap);
		glBindTexture(GL_TEXTURE_2D, shadowMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);

		// Disable writes to the color buffer
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (Status != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "FB error, status: 0x" << std::hex << Status << std::endl;
			return false;
		}

		return true;
	}

	void bindW()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	}

	void bindR(GLenum textureUnit)
	{
		glActiveTexture(textureUnit);
		glBindTexture(GL_TEXTURE_2D, shadowMap);
	}

private:
	GLuint FBO;
	GLuint shadowMap;
};