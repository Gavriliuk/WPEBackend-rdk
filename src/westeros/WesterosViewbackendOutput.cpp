#include "WesterosViewbackendOutput.h"

#include <cstdio>
#include <cstring>
#include <cassert>
#include <cstring>
#include <linux/input.h>
#include <locale.h>
#include <sys/mman.h>
#include <unistd.h>
#include <wpe/wpe.h>

namespace Westeros {

static WstOutputNestedListener output_listener = {
    WesterosViewbackendOutput::handleGeometryCallback,
    WesterosViewbackendOutput::handleModeCallback,
    WesterosViewbackendOutput::handleDoneCallback,
    WesterosViewbackendOutput::handleScaleCallback
};

void WesterosViewbackendOutput::handleGeometryCallback( void *userData, int32_t x, int32_t y, int32_t mmWidth, int32_t mmHeight,
                                                 int32_t subPixel, const char *make, const char *model, int32_t transform )
{
}

struct ModeData
{
    void* userData;
    int32_t width;
    int32_t height;
};

void WesterosViewbackendOutput::handleModeCallback( void *userData, uint32_t flags, int32_t width, int32_t height, int32_t refreshRate )
{
    auto& me = *static_cast<WesterosViewbackendOutput*>(userData);
    if (!me.m_viewbackend || (flags != WesterosViewbackendModeCurrent))
        return;
    // We will send an internal size change, now we need to update the output
    // size of our nested compositor instance.  We have requested this compositor
    // instance to be a repeater, but on devices where repeating composition is not
    // supported by the Wayland-egl implementation, the compositor output size must
    // be updated.  If the compositor were a repeater, it will perform no rendering but
    // instead forward buffers to the upstream compositor and no compositor output size
    // change would be required (but should be done anyway).
    if (me.m_compositor )
    {
       WstCompositorSetOutputSize( me.m_compositor, width, height );
    }

    ModeData *modeData = new ModeData { userData, width, height };
    g_ptr_array_add(me.m_modeDataArray, modeData);

    g_main_context_invoke(me.m_mainContext, [](gpointer data) -> gboolean
    {
        ModeData *d = (ModeData*)data;

        auto& backend_output = *static_cast<WesterosViewbackendOutput*>(d->userData);
        backend_output.m_width = d->width;
        backend_output.m_height = d->height;
        wpe_view_backend_dispatch_set_size(backend_output.m_viewbackend, d->width, d->height);

        g_ptr_array_remove_fast(backend_output.m_modeDataArray, data);
        delete d;
        return G_SOURCE_REMOVE;
    }, modeData);
}

void WesterosViewbackendOutput::handleDoneCallback( void *UserData )
{
}

void WesterosViewbackendOutput::handleScaleCallback( void *UserData, int32_t scale )
{
}

WesterosViewbackendOutput::WesterosViewbackendOutput(struct wpe_view_backend* backend)
 : m_compositor(nullptr)
 , m_viewbackend(backend)
 , m_width(800)
 , m_height(600)
 , m_modeDataArray(g_ptr_array_sized_new(4))
 , m_mainContext(g_main_context_get_thread_default())
{
}

WesterosViewbackendOutput::~WesterosViewbackendOutput()
{
    m_compositor = nullptr;
    m_viewbackend = nullptr;

    clearDataArray();
}

void WesterosViewbackendOutput::initializeNestedOutputHandler(WstCompositor *compositor)
{
    m_compositor = compositor;

    if (m_compositor && m_viewbackend) {
        if (!WstCompositorSetOutputNestedListener(m_compositor, &output_listener, this )) {
            fprintf(stderr, "ViewBackendWesteros: failed to set output listener: %s\n",
                WstCompositorGetLastErrorDetail(m_compositor));
        }
    }
}

void WesterosViewbackendOutput::initializeClient()
{
    wpe_view_backend_dispatch_set_size(m_viewbackend, m_width, m_height);
}

static void clearArray(GPtrArray *array)
{
    if (array->len)
    {
        g_ptr_array_foreach(array, [](gpointer data, gpointer user_data)
        {
            g_source_remove_by_user_data(data);
            delete (ModeData*)data;
        }, nullptr);
    }
    g_ptr_array_free(array, TRUE);
}

void WesterosViewbackendOutput::clearDataArray()
{
    if (g_main_context_is_owner(m_mainContext))
    {
        clearArray(m_modeDataArray);
    }
    else
    {
        g_main_context_invoke(m_mainContext, [](gpointer data) -> gboolean
        {
            clearArray((GPtrArray*)data);
            return G_SOURCE_REMOVE;
        }, m_modeDataArray);
    }
}

} // namespace Westeros
