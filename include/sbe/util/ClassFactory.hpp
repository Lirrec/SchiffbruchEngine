#ifndef COMPONENTFACTORY_HPP
#define COMPONENTFACTORY_HPP

#include <memory>
#include <typeinfo>
#include <typeindex>

namespace sbe
{



	template <class T>
	class Factory
	{
		public:
			virtual std::shared_ptr<T> create() = 0;
			virtual std::type_index type() = 0;
	};

	template <class T>
	class RealFactory : public Factory<T>
	{
		public:
			std::shared_ptr<T> create() override { return std::make_shared<T>(); }
			virtual std::type_index type() override {return std::type_index(typeid(T));}
	};

	template <class T, class Base>
	class  FactoryWithBase : public Factory<Base>
	{
		public:
			std::shared_ptr<Base> create() override { return std::dynamic_pointer_cast<Base>(std::make_shared<T>()); }
			std::type_index type() override { return std::type_index(typeid(T)); }
	};

	template <class T>
	class CopyFactory
	{
		public:
			virtual T createCopy() = 0;
	};

	template <class T, class U>
	class AssignmentConversionFactory : public CopyFactory<U>
	{
		public:
			virtual U createCopy() override { return T(); }
	};


} // namespace sbe

#endif // COMPONENTFACTORY_HPP
