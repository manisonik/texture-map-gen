#include "stdafx.h"
#include "msk-glm-camera.h"

using namespace Manisonik;

MCamera::MCamera()
{
	m_vecPos = glm::vec3(0.0, 0.0, 0.0f);
	m_vecFront = glm::vec3(0.0f, 0.0f, -1.0f);
	m_vecUp = glm::vec3(0.0f, 1.0f, 0.0f);
	m_matModel = glm::mat4(1.0f);
	m_matMVP = glm::mat4(1.0f);
	m_mouseSensitivity = 0.5f;
	m_FOV = 45.0f;
	m_fSpeed = 1.0f;
	m_Yaw = -90.0f;
	m_Pitch = 0.0f;
	m_bFirstMouse = true;
	m_fZoom = 1.0f;
	m_mouseLastX = 0;
	m_mouseLastY = 0;
	m_bMoveLeft = false;
	m_bMoveRight = false;
	m_bMoveForward = false;
	m_bMoveBackward = false;
	m_nMode = 3;

	m_pRay = std::make_shared<MRay>(glm::vec3(0, 0, 0));
}

MCamera::~MCamera() {}

void MCamera::SetPosition(glm::vec3 pos)
{
	m_vecPos = pos;
}

void MCamera::MoveLeft(bool bMove)
{
	m_bMoveLeft = bMove;
}

void MCamera::MoveRight(bool bMove)
{
	m_bMoveRight = bMove;
}

void MCamera::MoveBackward(bool bMove)
{
	m_bMoveBackward = bMove;
}

void MCamera::MoveForward(bool bMove)
{
	m_bMoveForward = bMove;
}

void MCamera::Look(float mouseX, float mouseY, GLboolean mouseDown) {
	// Store mouse positions
	m_mouseX = mouseX;
	m_mouseY = mouseY;

	if (m_bFirstMouse)
	{
		m_mouseLastX = mouseX;
		m_mouseLastY = mouseY;
		m_bFirstMouse = false;
	}

	if (mouseDown) {
		float mouseOffsetX = (mouseX - m_mouseLastX);
		float mouseOffsetY = (m_mouseLastY - mouseY);

		mouseOffsetX *= m_mouseSensitivity;
		mouseOffsetY *= m_mouseSensitivity;

		m_Yaw += mouseOffsetX;
		m_Pitch += mouseOffsetY;

		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_vecFront = glm::normalize(front);
	}

	m_mouseLastX = mouseX;
	m_mouseLastY = mouseY;
}

glm::vec3& MCamera::GetPosition() 
{
	return m_vecPos;
}

glm::mat4& MCamera::GetModelViewProjectionMatrix()
{
	return m_matMVP;
}

glm::mat4& MCamera::GetModelMatrix()
{
	return m_matModel;
}

glm::mat4& MCamera::GetViewMatrix()
{
	return m_matView;
}

glm::mat4& MCamera::GetProjectionMatrix()
{
	return m_matProjection;
}

void MCamera::Pan(glm::vec2 units) {
	m_vecPos += glm::vec3(units, 1.0f) * glm::normalize(glm::cross(m_vecFront, m_vecUp));
}

void MCamera::Zoom(GLfloat amount) {
	m_fZoom = amount;
}

void MCamera::Update(GLfloat dt)
{
	if (m_bMoveLeft) {
		m_vecPos -= glm::normalize(glm::cross(m_vecFront, m_vecUp)) * m_fSpeed * dt;
	}

	if (m_bMoveRight) {
		m_vecPos += glm::normalize(glm::cross(m_vecFront, m_vecUp)) * m_fSpeed * dt;
	}

	if (m_bMoveForward) {
		m_vecPos += m_fSpeed * dt * m_vecFront;
	}

	if (m_bMoveBackward) {
		m_vecPos -= m_fSpeed * dt* m_vecFront;
	}

	m_matView = glm::lookAt(
		m_vecPos,
		m_vecPos + m_vecFront,
		m_vecUp
	);

	m_matMVP = m_matProjection * m_matView * m_matModel;
}

void MCamera::SetViewport(int x, int y, int width, int height)
{
	// Prevent a divide by zero
	if (height == 0) {
		height = 1;
	}

	// Store width and height
	m_nWidth = width;
	m_nHeight = height;

	glViewport(x, y, width, height);
	
	if (m_nMode == 0)
	{
		m_matProjection = glm::ortho(-(float)width/2.0f, (float)width/2.0f, -(float)height/2.0f, (float)height/2.0f, -1.0f, 1.0f);
	}
	else if (m_nMode == 1)
	{
		m_matProjection = glm::ortho(0.0f,(float)width, (float)height, 0.0f, -1.0f, 1.0f);
	}
	else
	{
		float fAspect = (float)width / (float)height;
		m_matProjection = glm::perspective(
			glm::radians(m_FOV),
			fAspect,
			0.1f,
			1000.0f
		);
	}
}

void MCamera::SetMode(GLuint mode)
{
	this->m_nMode = mode;
}

void MCamera::SetModelMatrix(glm::mat4 modelMatrix)
{
	m_matModel = modelMatrix;
}

std::shared_ptr<MRay>& MCamera::GetRay()
{
	GLfloat x = m_mouseX;
	GLfloat y = m_nHeight - m_mouseY;

	glm::vec3 ray_near = glm::unProject(glm::vec3(x, y, -1.0f),
		m_matView, m_matProjection, glm::vec4(0.0, 0.0, m_nWidth, m_nHeight));

	glm::vec3 ray_far  = glm::unProject(glm::vec3(x, y, 1.0f),
		m_matView, m_matProjection, glm::vec4(0.0, 0.0, m_nWidth, m_nHeight));

	glm::vec3 ray_dir = glm::normalize(ray_far - ray_near);

	m_pRay->SetOrigin(ray_near);
	m_pRay->SetDirection(ray_dir);

	return m_pRay;
}
