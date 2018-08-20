#include "DistrhoUI.hpp"

#include "WolfSpectrumUI.hpp"
#include "Window.hpp"
#include "Config.hpp"
#include "Layout.hpp"
#include "Fonts/chivo_bold.hpp"
#include "WolfSpectrumParameters.hpp"

#include <string>

#if defined(DISTRHO_OS_WINDOWS)
#include "windows.h"
#endif

START_NAMESPACE_DISTRHO

WolfSpectrumUI::WolfSpectrumUI() : UI(300, 200)
{
    const uint minWidth = 300;
    const uint minHeight = 200;

    const uint knobsLabelBoxWidth = 66;
    const uint knobsLabelBoxHeight = 21;

    loadSharedResources();

    using namespace WOLF_FONTS;
    NanoVG::FontId chivoBoldId = createFontFromMemory("chivo_bold", (const uchar *)chivo_bold, chivo_bold_size, 0);
    NanoVG::FontId dejaVuSansId = findFont(NANOVG_DEJAVU_SANS_TTF);

    WolfSpectrumConfig::load();

    tryRememberSize();
    getParentWindow().saveSizeAtExit(true);

    const float width = getWidth();
    const float height = getHeight();

    fResizeHandle = new ResizeHandle(this, Size<uint>(18, 18));
    fResizeHandle->setCallback(this);
    fResizeHandle->setMinSize(minWidth, minHeight);

    fSpectrogram = new Spectrogram(this, this, Size<uint>(width, height));
    positionWidgets(width, height);
}

WolfSpectrumUI::~WolfSpectrumUI()
{
}

void WolfSpectrumUI::tryRememberSize()
{
    int width, height;
    FILE *file;
    std::string tmpFileName = PLUGIN_NAME ".tmp";

#if defined(DISTRHO_OS_WINDOWS)
    CHAR tempPath[MAX_PATH + 1];

    GetTempPath(MAX_PATH + 1, tempPath);
    std::string path = std::string(tempPath) + tmpFileName;
    file = fopen(path.c_str(), "r");
#else
    file = fopen(("/tmp/" + tmpFileName).c_str(), "r");
#endif

    if (file == NULL)
        return;

    const int numberScanned = fscanf(file, "%d %d", &width, &height);

    if (numberScanned == 2 && width && height)
    {
        setSize(width, height);
    }

    fclose(file);
}

void WolfSpectrumUI::positionWidgets(uint width, uint height)
{
    fResizeHandle->setAbsolutePos(width - fResizeHandle->getWidth(), height - fResizeHandle->getHeight());
}

void WolfSpectrumUI::parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case paramOutLeft:
        break;
    case paramOutRight:
        break;
    default:
        break;
    }
}

void WolfSpectrumUI::onNanoDisplay()
{
    const float width = getWidth();
    const float height = getHeight();
}

void WolfSpectrumUI::uiIdle()
{
    repaint();

    if (WolfSpectrumPlugin *const dspPtr = (WolfSpectrumPlugin *)getPluginInstancePointer())
    {
        if (dspPtr->fSamples == nullptr)
            return;

        const MutexLocker csm(dspPtr->fMutex);

        fSpectrogram->setSamples(dspPtr->fSamples);
    }
}

bool WolfSpectrumUI::onMouse(const MouseEvent &ev)
{
    return false;
}

void WolfSpectrumUI::uiReshape(uint width, uint height)
{
    positionWidgets(width, height);
}

bool WolfSpectrumUI::onKeyboard(const KeyboardEvent &ev)
{
    return false;
}

void WolfSpectrumUI::nanoSwitchClicked(NanoSwitch *nanoSwitch)
{
    const uint switchId = nanoSwitch->getId();
    const int value = nanoSwitch->isDown() ? 1 : 0;

    setParameterValue(switchId, value);
}

void WolfSpectrumUI::nanoButtonClicked(NanoButton *nanoButton)
{
}

void WolfSpectrumUI::nanoWheelValueChanged(NanoWheel *nanoWheel, const int value)
{
    const uint id = nanoWheel->getId();

    setParameterValue(id, value);
}

void WolfSpectrumUI::nanoKnobValueChanged(NanoKnob *nanoKnob, const float value)
{
    const uint id = nanoKnob->getId();

    setParameterValue(id, value);
}

void WolfSpectrumUI::resizeHandleMoved(int width, int height)
{
    setSize(width, height);
}

UI *createUI()
{
    return new WolfSpectrumUI();
}

END_NAMESPACE_DISTRHO