#pragma once

#include <android/hardware/myvibrator/1.0/IMyVibrator.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

// add 1
#include <hardware/hardware.h>
#include <hardware/vibrator.h>

namespace android {
namespace hardware {
namespace myvibrator {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct MyVibrator : public IMyVibrator {
    // add 2
    vibrator_device_t *mDevice;
    MyVibrator(vibrator_device_t *device);

    Return<::android::hardware::myvibrator::V1_0::MyVibratorRet> on(uint32_t timeoutMs) override;
    Return<::android::hardware::myvibrator::V1_0::MyVibratorRet> off() override;
};

}
}
}
}
}