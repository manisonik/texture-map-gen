#include "stdafx.h"
#include "msk-glm-camera.h"

using namespace TexGen;

TgCamera::TgCamera() :
	m_ray(glm::vec3(0, 0, 0)),
	m_vecPos(glm::vec3(0.0, 0.0, 0.0f)),
	m_vecFront(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_vecUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_matModel(glm::mat4(1.0f)),
	m_matMVP(glm::mat4(1.0f)),
	m_mouseSensitivity(0.5f),
	m_fov(45.0f),
	m_speed(1.0f),
	m_yaw(-90.0f),
	m_pitch(0.0f),
	m_firstMouse(true),
	m_zoom(1.0f),
	m_mouseLastX(0),
	m_mouseLastY(0),
	m_moveLeft(false),
	m_moveRight(false),
	m_moveForward(false),
	m_moveBackward(false),
	m_mode(3)
{
}

TgCamera::~TgCamera() {}

void TgCamera::setPosition(const glm::vec3& pos)
{
	m_vecPos = pos;
}

void TgCamera::moveLeft(bool bMove)
{
	m_moveLeft = bMove;
}

void TgCamera::moveRight(bool bMove)
{
	m_moveRight = bMove;
}

void TgCamera::moveBackward(bool bMove)
{
	m_moveBackward = bMove;
}

void TgCamera::moveForward(bool bMove)
{
	m_moveForward = bMove;
}

void TgCamera::look(float mouseX, float mouseY, bool mouseDown) {
	// Store mouse positions
	m_mouseX = mouseX;
	m_mouseY = mouseY;

	if (m_firstMouse)
	{
		m_mouseLastX = mouseX;
		m_mouseLastY = mouseY;
		m_firstMouse = false;
	}

	if (mouseDown) {
		float mouseOffsetX = (mouseX - m_mouseLastX);
		float mouseOffsetY = (m_mouseLastY - mouseY);

		mouseOffsetX *= m_mouseSensitivity;
		mouseOffsetY *= m_mouseSensitivity;

		m_yaw += mouseOffsetX;
		m_pitch += mouseOffsetY;

		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		front.y = sin(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_vecFront = glm::normalize(front);
	}

	m_mouseLastX = mouseX;
	m_mouseLastY = mouseY;
}

const glm::vec3& TgCamera::getPosition() const
{
	return m_vecPos;
}

const glm::mat4& TgCamera::getModelViewProjectionMatrix() const
{
	return m_matMVP;
}

const glm::mat4& TgCamera::getModelMatrix() const
{
	return m_matModel;
}

const glm::mat4& TgCamera::getViewMatrix() const
{
	return m_matView;
}

const glm::mat4& TgCamera::getProjectionMatrix() const
{
	return m_matProjection;
}

void TgCamera::pan(const glm::vec2& units) {
	m_vecPos += glm::vec3(units, 1.0f) * glm::normalize(glm::cross(m_vecFront, m_vecUp));
}

void TgCamera::zoom(GLfloat amount) {
	m_zoom = amount;
}

void TgCamera::update(GLfloat dt)
{
	if (m_moveLeft) {
		m_vecPos -= glm::normalize(glm::cross(m_vecFront, m_vecUp)) * m_speed * dt;
	}

	if (m_moveRight) {
		m_vecPos += glm::normalize(glm::cross(m_vecFront, m_vecUp)) * m_speed * dt;
	}

	if (m_moveForward) {
		m_vecPos += m_speed * dt * m_vecFront;
	}

	if (m_moveBackward) {
		m_vecPos -= m_speed * dt* m_vecFront;
	}

	m_matView = glm::lookAt(
		m_vecPos,
		m_vecPos + m_vecFront,
		m_vecUp
	);

	m_matMVP = m_matProjection * m_matView * m_matModel;
}

void TgCamera::setViewport(int x, int y, int width, int height)
{
	// Prevent a divide by zero
	if (height == 0) {
		height = 1;
	}

	// Store width and height
	m_width = width;
	m_height = height;

	glViewport(x, y, width, height);
	
	if (m_mode == 0)
	{
		m_matProjection = glm::ortho(-(float)width/2.0f, (float)width/2.0f, -(float)height/2.0f, (float)height/2.0f, -1.0f, 1.0f);
	}
	else if (m_mode == 1)
	{
		m_matProjection = glm::ortho(0.0f,(float)width, (float)height, 0.0f, -1.0f, 1.0f);
	}
	else
	{
		float fAspect = (float)width / (float)height;
		m_matProjection = glm::perspective(
			glm::radians(m_fov),
			fAspect,
			0.1f,
			1000.0f
		);
	}
}

void TgCamera::setMode(unsigned int mode)
{
	m_mode = mode;
}

void TgCamera::setModelMatrix(const glm::mat4& modelMatrix)
{
	m_matModel = modelMatrix;
}

const TgRay& TgCamera::getRay()
{
	float x = m_mouseX;
	float y = m_height - m_mouseY;

	glm::vec3 ray_near = glm::unProject(glm::vec3(x, y, -1.0f),
		m_matView, m_matProjection, glm::vec4(0.0, 0.0, m_width, m_height));

	glm::vec3 ray_far  = glm::unProject(glm::vec3(x, y, 1.0f),
		m_matView, m_matProjection, glm::vec4(0.0, 0.0, m_width, m_height));

	glm::vec3 ray_dir = glm::normalize(ray_far - ray_near);

	m_ray.setOrigin(ray_near);
	m_ray.setDirection(ray_dir);

	return m_ray;
}
