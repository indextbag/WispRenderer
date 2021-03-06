/*!
 * Copyright 2019 Breda University of Applied Sciences and Team Wisp (Viktor Zoutman, Emilio Laiso, Jens Hagen, Meine Zeinstra, Tahar Meijs, Koen Buitenhuis, Niels Brunekreef, Darius Bouma, Florian Schut)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once
#include <stdint.h>
#include <vector>

namespace wr
{

	class IDFactory
	{
	public:

		IDFactory();
		virtual ~IDFactory() = default;

		IDFactory(IDFactory const &) = delete;
		IDFactory& operator=(IDFactory const &) = delete;
		IDFactory(IDFactory&&) = delete;
		IDFactory& operator=(IDFactory&&) = delete;

		void MakeIDAvailable(std::uint32_t unused_id);
		std::uint32_t GetUnusedID();

	protected:

		std::uint32_t m_id;
		std::vector<uint32_t> m_unused_ids;
	};

} /* wr */