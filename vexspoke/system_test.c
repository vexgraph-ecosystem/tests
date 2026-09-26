#include <stdio.h>
#include <inttypes.h>

#include "system/system.h"

int main(void) {
    printf("=== Vex System Discovery Probe ===\n\n");

    // Force discovery or query lazily
    SystemDiscovery_bootstrap();

    // 1. Hardware Info
    printf("[Hardware Information]\n");
    printf("  Device Model : %s\n", HardwareInfo_getDeviceModel());
    printf("  CPU Brand    : %s\n", HardwareInfo_getCpuBrand());
    printf("  CPU Cores    : %d cores, %d threads\n",
           HardwareInfo_getCpuCoreCount(), HardwareInfo_getCpuThreadCount());
    printf("  Total RAM    : %.2f GB (%" PRIu64 " bytes)\n",
           (double)HardwareInfo_getRamTotal() / (1024.0 * 1024.0 * 1024.0),
           HardwareInfo_getRamTotal());
    if (HardwareInfo_hasBattery()) {
        printf("  Battery      : %.0f%% (%s)\n",
               HardwareInfo_getBatteryLevel() * 100.0f,
               HardwareInfo_getBatteryStatus());
    } else {
        printf("  Power Source : AC / Desktop (No Battery Installed)\n");
    }
    printf("  OS / Arch    : %s (%s)\n\n",
           HardwareInfo_getOperatingSystem(), HardwareInfo_getSystemArchitecture());

    // 2. Graphics Info
    printf("[Graphics Information]\n");
    printf("  GPU Name     : %s\n", GraphicsInfo_getGpuName());
    printf("  Graphics API : %s\n", GraphicsInfo_getPrimaryGraphicsApi());
    printf("  Unified Mem  : %s\n", GraphicsInfo_getUnifiedMemoryEnabled() ? "Yes" : "No");
    printf("  VRAM Budget  : %.2f GB (%" PRIu64 " bytes)\n\n",
           (double)GraphicsInfo_getVramTotal() / (1024.0 * 1024.0 * 1024.0),
           GraphicsInfo_getVramTotal());

    // 3. Display Subsystem (Points vs Physical Pixels vs Native Panel Grid)
    printf("[Display Subsystem]\n");
    int count = DisplayInfo_getMonitorCount();
    printf("  Connected Monitors : %d\n", count);
    printf("  Primary Resolution :\n");
    printf("    Logical Points   : %d x %d pt\n",
           DisplayInfo_getPointResolutionWidth(), DisplayInfo_getPointResolutionHeight());
    printf("    Physical Pixels  : %d x %d px (Active Framebuffer Mode)\n",
           DisplayInfo_getMonitorResolutionWidth(), DisplayInfo_getMonitorResolutionHeight());
    printf("    Native Panel     : %d x %d px (Hardware Physical Panel Grid)\n",
           DisplayInfo_getNativeResolutionWidth(), DisplayInfo_getNativeResolutionHeight());
    printf("    Display Density  : %.2fx (Scale Factor)\n", DisplayInfo_getDisplayDensity());
    printf("    Hardware Density : %.3fx (Native Panel Scale)\n", DisplayInfo_getHardwareDensity());
    printf("    Refresh Rate     : %d Hz\n", DisplayInfo_getCurrentRefreshRate());
    printf("    HDR Supported    : %s\n\n", DisplayInfo_getHdrSupported() ? "Yes" : "No");

    // List all monitors
    size_t monitorCount = 0;
    DisplayMonitor **monitors = DisplayInfo_getMonitors(&monitorCount);
    for (size_t i = 0; i < monitorCount; i++) {
        DisplayMonitor *m = monitors[i];
        if (!m) continue;
        printf("  [Monitor #%zu] %s (ID: %u)\n", i, DisplayMonitor_getName(m), DisplayMonitor_getId(m));
        printf("    Logical Points : %d x %d pt\n",
               DisplayMonitor_getPointWidth(m), DisplayMonitor_getPointHeight(m));
        printf("    Physical Pixels: %d x %d px\n",
               DisplayMonitor_getCurrentWidth(m), DisplayMonitor_getCurrentHeight(m));
        printf("    Native Panel   : %d x %d px\n",
               DisplayMonitor_getNativeWidth(m), DisplayMonitor_getNativeHeight(m));
        printf("    Scale / Density: %.2fx\n", DisplayMonitor_getDpi(m));
        printf("    Refresh Rate   : %d Hz\n", DisplayMonitor_getRefreshRate(m));
        printf("    HDR Capable    : %s\n", DisplayMonitor_getHdrSupported(m) ? "Yes" : "No");
    }

    printf("\n=== System Discovery Complete ===\n");
    return 0;
}
