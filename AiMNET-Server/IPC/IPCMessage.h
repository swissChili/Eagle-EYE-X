#pragma once

#include <stdint.h>

namespace IPC
{

#pragma pack(push, 1)

	enum class MessageType : uint8_t
	{
		BODY = 0,
		LOG = 1,
		BATCH_DONE = 2,
		TIME = 3,
	};

	const uint32_t messageBufferLen = 22;

	struct Message
	{
		MessageType type;

		union
		{
			struct
			{
				uint16_t x, y, width, height, headX, headY;
			} body;

			struct
			{
				uint8_t length;
				char message[messageBufferLen];
			} log;

			struct
			{
				uint32_t ms;
			} time;
		};
	};

	const uint32_t messageSize = sizeof(Message);
	

	static_assert(messageSize == 24);

#pragma pack(pop)

}
