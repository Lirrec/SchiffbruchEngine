#pragma once

namespace sbe
{
	class ImGuiWidget
	{
	public:
		virtual ~ImGuiWidget() = default;
		virtual void renderImGui() = 0;
	};
} // namespace sbe
