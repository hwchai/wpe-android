/**
 * Copyright (C) 2022 Igalia S.L. <info@igalia.com>
 *   Author: Fernando Jimenez Moreno <fjimenez@igalia.com>
 *   Author: Zan Dobersek <zdobersek@igalia.com>
 *   Author: Jani Hautakangas <jani@igalia.com>
 *   Author: Loïc Le Page <llepage@igalia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#pragma once

#include "MessagePump.h"

#include <wpe/webkit.h>

class Browser final {
public:
    static void configureJNIMappings();

    static Browser& instance() noexcept
    {
        static Browser s_singleton;
        return s_singleton;
    }

    Browser(Browser&&) = delete;
    Browser& operator=(Browser&&) = delete;
    Browser(const Browser&) = delete;
    Browser& operator=(const Browser&) = delete;

    ~Browser() { jniShut(); }

    WebKitWebContext* webContext() const noexcept { return m_webContext.get(); }

    void invokeOnUiThread(void (*onExec)(void*), void (*onDestroy)(void*), void* userData) const noexcept;

private:
    Browser() = default;

    friend class JNIBrowserCache;
    void jniInit(const char* dataDir, const char* cacheDir);
    void jniShut() noexcept;

    template <typename T> using ProtectedUniquePointer = std::unique_ptr<T, std::function<void(T*)>>;

    std::unique_ptr<MessagePump> m_messagePump {};
    ProtectedUniquePointer<WebKitWebsiteDataManager> m_websiteDataManager {};
    ProtectedUniquePointer<WebKitWebContext> m_webContext {};
};
