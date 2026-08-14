#ifndef B0E55494_8528_4393_9395_86CB1F6922FA
#define B0E55494_8528_4393_9395_86CB1F6922FA

namespace nevonex
{
namespace notify
{

enum class DataUploadStatus
{
    FEATURE_DATA_UPLOAD_ALLOWED,
    FEATURE_DATA_UPLOAD_BLOCKED
};

typedef void (*DataUploadStatusNotifierCallback)(DataUploadStatus);

DataUploadStatusNotifierCallback dataUploadStatusNotifierCallback = nullptr; //  NOSONAR - cpp:S5421 This value cannot be const

void setDataUploadStatusNotifierCallback(DataUploadStatusNotifierCallback callback)
{
    dataUploadStatusNotifierCallback = callback;
}

}
}

#endif /* B0E55494_8528_4393_9395_86CB1F6922FA */
