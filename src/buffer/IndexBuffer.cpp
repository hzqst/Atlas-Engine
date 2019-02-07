#include "IndexBuffer.h"

namespace Atlas {

    namespace Buffer {

        IndexBuffer::IndexBuffer(uint32_t dataType, size_t elementSize, size_t elementCount) :
                Buffer(AE_INDEX_BUFFER, elementSize, AE_BUFFER_IMMUTABLE), dataType(dataType) {

            SetSize(elementCount);

        }

        IndexBuffer::~IndexBuffer() {



        }

        void IndexBuffer::SetData(void *data, size_t offset, size_t length) {

            auto stagingBuffer = Buffer(AE_STAGING_BUFFER, elementSize, AE_BUFFER_DYNAMIC_STORAGE);
            stagingBuffer.SetSize(length);

            // We don't need to bind because it is already bound by previous operations
            stagingBuffer.SetData(data, 0, length);

            Copy(&stagingBuffer, 0, offset * elementSize, length * elementSize);

        }

        uint32_t IndexBuffer::GetDataType() {

            return dataType;

        }

    }

}