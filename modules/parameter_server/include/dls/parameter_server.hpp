#ifndef PARAMETER_SERVER_HPP_05f2fd08_3966_43ee_b7c8_5c344e7afd3f
#define PARAMETER_SERVER_HPP_05f2fd08_3966_43ee_b7c8_5c344e7afd3f

#include <memory>
#include "dls2/util/service/service.hpp"

namespace dls
{
	namespace impl
	{
		struct ParameterServerImpl;
	}
	class ParameterServer
	{
	public:
		ParameterServer();

	private:
		std::unique_ptr<impl::ParameterServerImpl> pimpl;
	};
}

#endif // PARAMETER_SERVER_HPP_05f2fd08_3966_43ee_b7c8_5c344e7afd3f
