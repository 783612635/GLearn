#pragma once
#include <vector>
#include <functional>
#include <string>
#include <iostream>

namespace test
{
	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class TestMenu:public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering test " << name << std::endl;
			m_Tests.push_back(std::make_pair(name, []() {
				return new T();
				}));
		}

	private:
		//我们希望每个Test都能有一个名字标签（for imgui），此外，同时返回该Test*
		std::vector < std::pair<std::string, std::function<Test* ()>>> m_Tests;
		Test*& m_CurrentTest;
	};
}