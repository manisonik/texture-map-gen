#pragma once
#include "msk-glm-ray.h"

namespace TexGen {
	class TgCamera
	{
	private:
		glm::mat4 m_matProjection;
		glm::mat4 m_matView;
		glm::mat4 m_matModel;
		glm::mat4 m_matMVP;
		glm::vec3 m_vecUp;
		glm::vec3 m_vecCenter;
		glm::vec3 m_vecEye;
		glm::vec3 m_vecPos;
		glm::vec3 m_vecTarget;
		glm::vec3 m_vecRight;
		glm::vec3 m_vecDirection;
		glm::vec3 m_vecFront;

		float m_mouseSensitivity;
		float m_mouseX;
		float m_mouseY;
		float m_mouseLastX;
		float m_mouseLastY;
		float m_yaw;
		float m_pitch;
		float m_fov;
		bool m_moveLeft;
		bool m_moveRight;
		bool m_moveForward;
		bool m_moveBackward;
		float m_speed;
		bool m_firstMouse;
		float m_zoom;
		unsigned int m_mode;

		unsigned int m_width;
		unsigned int m_height;

		TgRay m_ray;
	public:
		TgCamera();
		~TgCamera();

		void setPosition(const glm::vec3& pos);
		void moveLeft(bool bMove);
		void moveRight(bool bMove);
		void moveBackward(bool bMove);
		void moveForward(bool bMove);
		void look(float mouseX, float mouseY, bool mouseDown);
		void update(float dt);
		void setViewport(int x, int y, int width, int height);
		void setMode(unsigned int mode);
		void setModelMatrix(const glm::mat4& modelMatrix);

		const glm::vec3& getPosition() const;
		const glm::mat4& getModelViewProjectionMatrix() const;
		const glm::mat4& getModelMatrix() const;
		const glm::mat4& getViewMatrix() const;
		const glm::mat4& getProjectionMatrix() const;
		void pan(const glm::vec2& units);
		void zoom(float amount);
		const TgRay& getRay();
	};
}

