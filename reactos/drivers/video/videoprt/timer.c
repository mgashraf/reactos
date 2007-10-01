/*
 * VideoPort driver
 *
 * Copyright (C) 2002, 2003, 2004 ReactOS Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; see the file COPYING.LIB.
 * If not, write to the Free Software Foundation,
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * $Id: timer.c 21844 2006-05-07 19:34:23Z ion $
 */

#include "videoprt.h"

/* PRIVATE FUNCTIONS **********************************************************/

VOID NTAPI
IntVideoPortTimerRoutine(
   IN PDEVICE_OBJECT DeviceObject,
   IN PVOID ServiceContext)
{
   PVIDEO_PORT_DEVICE_EXTENSION DeviceExtension = ServiceContext;

   ASSERT(DeviceExtension->DriverExtension->InitializationData.HwTimer != NULL);

   DeviceExtension->DriverExtension->InitializationData.HwTimer(
      &DeviceExtension->MiniPortDeviceExtension);
}

BOOLEAN NTAPI
IntVideoPortSetupTimer(
   IN PDEVICE_OBJECT DeviceObject,
   IN PVIDEO_PORT_DRIVER_EXTENSION DriverExtension)
{
   NTSTATUS Status;
   PVIDEO_PORT_DEVICE_EXTENSION DeviceExtension;

   DeviceExtension = (PVIDEO_PORT_DEVICE_EXTENSION)DeviceObject->DeviceExtension;

   if (DriverExtension->InitializationData.HwTimer != NULL)
   {
      DPRINT("Initializing timer\n");

      Status = IoInitializeTimer(
         DeviceObject,
         IntVideoPortTimerRoutine,
         DeviceExtension);

      if (!NT_SUCCESS(Status))
      {
         DPRINT("IoInitializeTimer failed with status 0x%08x\n", Status);
         return FALSE;
      }
   }

   return TRUE;
}

/* PUBLIC FUNCTIONS ***********************************************************/

/*
 * @implemented
 */

VOID NTAPI
VideoPortStartTimer(IN PVOID HwDeviceExtension)
{
   DPRINT("VideoPortStartTimer\n");
   IoStartTimer(VIDEO_PORT_GET_DEVICE_EXTENSION(HwDeviceExtension)->FunctionalDeviceObject);
}

/*
 * @implemented
 */

VOID NTAPI
VideoPortStopTimer(IN PVOID HwDeviceExtension)
{
   DPRINT("VideoPortStopTimer\n");
   IoStopTimer(VIDEO_PORT_GET_DEVICE_EXTENSION(HwDeviceExtension)->FunctionalDeviceObject);
}
