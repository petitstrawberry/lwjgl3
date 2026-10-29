/*
 * Copyright LWJGL. All rights reserved.
 * License terms: https://www.lwjgl.org/license
 */
package org.lwjgl.system.scarlet;

import org.jspecify.annotations.*;
import org.lwjgl.system.*;

import java.nio.*;

import static org.lwjgl.system.Checks.*;
import static org.lwjgl.system.MemoryStack.*;
import static org.lwjgl.system.MemoryUtil.*;

/** A native Scarlet library loaded by the process dynamic loader. */
public class ScarletLibrary extends SharedLibrary.Default {

    static {
        Library.initialize();
        // Resolve this JNI method before a failed load sets the loader error.
        // Lazy JNI symbol lookup otherwise overwrites that error with the
        // lookup for ndlerror itself.
        ndlerror();
    }

    public ScarletLibrary(String name) {
        super(name, loadLibrary(name));
    }

    private static long loadLibrary(String name) {
        try (MemoryStack stack = stackPush()) {
            long handle = ndlopen(memAddress(stack.UTF8(name)));
            if (handle == NULL) {
                throw new UnsatisfiedLinkError("Failed to load library: " + name + " (" + ndlerror() + ")");
            }
            return handle;
        }
    }

    @Override
    public @Nullable String getPath() {
        return SharedLibraryUtil.getLibraryPath(address());
    }

    @Override
    public long getFunctionAddress(ByteBuffer functionName) {
        if (CHECKS) {
            checkNT1(functionName);
        }
        return ndlsym(address(), memAddress(functionName));
    }

    @Override
    public void free() {
        if (ndlclose(address()) != 0) {
            throw new IllegalStateException("Failed to close library: " + ndlerror());
        }
    }

    private static native long ndlopen(long name);
    private static native long ndlsym(long handle, long name);
    private static native int ndlclose(long handle);
    private static native @Nullable String ndlerror();
}
