// Copyright (C) Codeplay Software Limited
//
// Licensed under the Apache License, Version 2.0 (the "License") with LLVM
// Exceptions; you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://github.com/codeplaysoftware/oneapi-construction-kit/blob/main/LICENSE.txt
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations
// under the License.
//
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <CL/cl.h>
#include <CL/cl_ext_codeplay_host.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IS_CL_SUCCESS(X)                                                       \
  {                                                                            \
    cl_int ret_val = X;                                                        \
    if (CL_SUCCESS != ret_val) {                                               \
      fprintf(stderr, "OpenCL error occurred: %s returned %d\n", #X, ret_val); \
      exit(1);                                                                 \
    }                                                                          \
  }

void printUsage(const char *arg0) {
  printf("usage: %s [-h] [--platform <name>] [--device <name>]\n", arg0);
}

void parseArguments(const int argc, const char **argv,
                    const char **platform_name, const char **device_name) {
  for (int argi = 1; argi < argc; argi++) {
    if (0 == strcmp("-h", argv[argi]) || 0 == strcmp("--help", argv[argi])) {
      printUsage(argv[0]);
      exit(0);
    } else if (0 == strcmp("--platform", argv[argi])) {
      argi++;
      if (argi == argc) {
        printUsage(argv[0]);
        fprintf(stderr, "expected platform name\n");
        exit(1);
      }
      *platform_name = argv[argi];
    } else if (0 == strcmp("--device", argv[argi])) {
      argi++;
      if (argi == argc) {
        printUsage(argv[0]);
        fprintf(stderr, "error: expected device name\n");
        exit(1);
      }
      *device_name = argv[argi];
    } else {
      printUsage(argv[0]);
      fprintf(stderr, "error: invalid argument: %s\n", argv[argi]);
      exit(1);
    }
  }
}

cl_platform_id selectPlatform(const char *platform_name_arg) {
  cl_uint num_platforms;
  IS_CL_SUCCESS(clGetPlatformIDs(0, NULL, &num_platforms));

  if (0 == num_platforms) {
    fprintf(stderr, "No OpenCL platforms found, exiting\n");
    exit(1);
  }

  cl_platform_id *platforms = malloc(sizeof(cl_platform_id) * num_platforms);
  if (NULL == platforms) {
    fprintf(stderr, "\nCould not allocate memory for platform ids\n");
    exit(1);
  }
  IS_CL_SUCCESS(clGetPlatformIDs(num_platforms, platforms, NULL));

  printf("Available platforms are:\n");

  unsigned selected_platform = 0;
  for (cl_uint i = 0; i < num_platforms; ++i) {
    size_t platform_name_size;
    IS_CL_SUCCESS(clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 0, NULL,
                                    &platform_name_size));

    if (0 == platform_name_size) {
      printf("  %u. Nameless platform\n", i + 1);
    } else {
      char *platform_name = malloc(platform_name_size);
      if (NULL == platform_name) {
        fprintf(stderr, "\nCould not allocate memory for platform name\n");
        exit(1);
      }
      IS_CL_SUCCESS(clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME,
                                      platform_name_size, platform_name, NULL));
      printf("  %u. %s\n", i + 1, platform_name);
      if (platform_name_arg && 0 == strcmp(platform_name, platform_name_arg)) {
        selected_platform = i + 1;
      }
      free(platform_name);
    }
  }

  if (platform_name_arg != NULL && selected_platform == 0) {
    fprintf(stderr, "Platform name matching '--platform %s' not found\n",
            platform_name_arg);
    exit(1);
  }

  if (1 == num_platforms) {
    printf("\nSelected platform 1\n");
    selected_platform = 1;
  } else if (0 != selected_platform) {
    printf("\nSelected platform %u by '--platform %s'\n", selected_platform,
           platform_name_arg);
  } else {
    printf("\nPlease select a platform: ");
    if (1 != scanf("%u", &selected_platform)) {
      fprintf(stderr, "\nCould not parse provided input, exiting\n");
      exit(1);
    }
  }

  selected_platform -= 1;

  if (num_platforms <= selected_platform) {
    fprintf(stderr, "\nSelected unknown platform, exiting\n");
    exit(1);
  } else {
    printf("\nRunning example on platform %u\n", selected_platform + 1);
  }

  cl_platform_id selected_platform_id = platforms[selected_platform];
  free(platforms);
  return selected_platform_id;
}

cl_device_id selectDevice(cl_platform_id selected_platform,
                          const char *device_name_arg) {
  cl_uint num_devices;

  IS_CL_SUCCESS(clGetDeviceIDs(selected_platform, CL_DEVICE_TYPE_ALL, 0, NULL,
                               &num_devices));

  if (0 == num_devices) {
    fprintf(stderr, "No OpenCL devices found, exiting\n");
    exit(1);
  }

  cl_device_id *devices = malloc(sizeof(cl_device_id) * num_devices);
  if (NULL == devices) {
    fprintf(stderr, "\nCould not allocate memory for device ids\n");
    exit(1);
  }
  IS_CL_SUCCESS(clGetDeviceIDs(selected_platform, CL_DEVICE_TYPE_ALL,
                               num_devices, devices, NULL));

  printf("Available devices are:\n");

  unsigned selected_device = 0;
  for (cl_uint i = 0; i < num_devices; ++i) {
    size_t device_name_size;
    IS_CL_SUCCESS(clGetDeviceInfo(devices[i], CL_DEVICE_NAME, 0, NULL,
                                  &device_name_size));

    if (0 == device_name_size) {
      printf("  %u. Nameless device\n", i + 1);
    } else {
      char *device_name = malloc(device_name_size);
      if (NULL == device_name) {
        fprintf(stderr, "\nCould not allocate memory for device name\n");
        exit(1);
      }
      IS_CL_SUCCESS(clGetDeviceInfo(devices[i], CL_DEVICE_NAME,
                                    device_name_size, device_name, NULL));
      printf("  %u. %s\n", i + 1, device_name);
      if (device_name_arg && 0 == strcmp(device_name, device_name_arg)) {
        selected_device = i + 1;
      }
      free(device_name);
    }
  }

  if (device_name_arg != NULL && selected_device == 0) {
    fprintf(stderr, "Device name matching '--device %s' not found\n",
            device_name_arg);
    exit(1);
  }

  if (1 == num_devices) {
    printf("\nSelected device 1\n");
    selected_device = 1;
  } else if (0 != selected_device) {
    printf("\nSelected device %u by '--device %s'\n", selected_device,
           device_name_arg);
  } else {
    printf("\nPlease select a device: ");
    if (1 != scanf("%u", &selected_device)) {
      fprintf(stderr, "\nCould not parse provided input, exiting\n");
      exit(1);
    }
  }

  selected_device -= 1;

  if (num_devices <= selected_device) {
    fprintf(stderr, "\nSelected unknown device, exiting\n");
    exit(1);
  } else {
    printf("\nRunning example on device %u\n", selected_device + 1);
  }

  cl_device_id selected_device_id = devices[selected_device];
  free(devices);
  return selected_device_id;
}

int main(const int argc, const char **argv) {
  const char *platform_name = NULL;
  const char *device_name = NULL;
  parseArguments(argc, argv, &platform_name, &device_name);

  /* Get the OpenCL platfrom */
  cl_platform_id selected_platform = selectPlatform(platform_name);
  /* Get the OpenCL device */
  cl_device_id selected_device = selectDevice(selected_platform, device_name);

  /* Get the extension entry point */
  clSetNumThreadsCODEPLAY_fn clSetNumThreadsCODEPLAY_ptr;

  clSetNumThreadsCODEPLAY_ptr = (clSetNumThreadsCODEPLAY_fn)(intptr_t)
      clGetExtensionFunctionAddressForPlatform(selected_platform,
                                               "clSetNumThreadsCODEPLAY");
  if (clSetNumThreadsCODEPLAY_ptr == NULL) {
    printf("Failed to get the entry point to cl_codeplay_set_threads\n");
    return 0;
  }

  /* Test the extension */
  if (clSetNumThreadsCODEPLAY_ptr(selected_device, 1) ==
      CL_DEVICE_NOT_AVAILABLE) {
    printf("Successfully called clSetNumThreadsCODEPLAY_ptr()\n");
  } else {
    printf("Failed to call clSetNumThreadsCODEPLAY_ptr()\n");
  }

  return 0;
}
