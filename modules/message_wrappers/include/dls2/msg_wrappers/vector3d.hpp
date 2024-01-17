
#ifndef VECTOR_THREE_DIM_HPP
#define VECTOR_THREE_DIM_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/vector3d.h"
#include <Eigen/Dense>

namespace dls
{
    class Vector3d : public Wrapper<Vector3dMsg>
    {
    public:
        template <typename SignalType>
        friend class SignalWriter;
        friend class PeriodicPluginBase;
        Vector3d();
        Vector3d(const Vector3d &Vector3d);
        virtual ~Vector3d();

        operator Vector3dMsg() const override;
        Vector3d &operator=(const Vector3dMsg &Vector3d_msg) override;
        Vector3d &operator=(const Vector3d &Vector3d);

        std::string frame_id_{};
        uint32_t sequence_id_{};
        double timestamp_{};

        Eigen::Vector3d data_;

    private:
        virtual void setDataFromWrapperBase(WrapperBase *wrapper_base) override;
    };
} // namespace dls

#endif