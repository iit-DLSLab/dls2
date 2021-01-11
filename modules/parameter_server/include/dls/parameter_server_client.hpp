#ifndef PARAMETER_SERVER_CLIENT_HPP_a016e6d9_35ec_43c4_a2ae_78a5d05710f0
#define PARAMETER_SERVER_CLIENT_HPP_a016e6d9_35ec_43c4_a2ae_78a5d05710f0

#include "dls/parameter_store.hpp"

#include <string>
#include <memory>

namespace dls
{
	namespace impl
	{
		struct ParameterServerClientImpl;
	}
	class ParameterServerClient
	{
	public:
		ParameterServerClient();
		void   setDouble(std::string const &key, double val);
		double getDouble(std::string const &key);

	private:
		std::unique_ptr<impl::ParameterServerClientImpl> pimpl;
	};
}

#endif // PARAMETER_SERVER_CLIENT_HPP_a016e6d9_35ec_43c4_a2ae_78a5d05710f0
