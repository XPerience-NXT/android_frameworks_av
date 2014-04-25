/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_HARDWARE_CAMERA_PARAMETERS_H
#define ANDROID_HARDWARE_CAMERA_PARAMETERS_H

#include <utils/KeyedVector.h>
#include <utils/String8.h>

namespace android {

struct Size {
    int width;
    int height;

    Size() {
        width = 0;
        height = 0;
    }

    Size(int w, int h) {
        width = w;
        height = h;
    }
};
#ifdef QCOM_HARDWARE
struct FPSRange{
    int minFPS;
    int maxFPS;

    FPSRange(){
        minFPS=0;
        maxFPS=0;
    };
    FPSRange(int min,int max){
        minFPS=min;
        maxFPS=max;
   };
};
#endif
class CameraParameters
{
public:
    CameraParameters();
    CameraParameters(const String8 &params) { unflatten(params); }
    ~CameraParameters();

    String8 flatten() const;
    void unflatten(const String8 &params);

    void set(const char *key, const char *value);
    void set(const char *key, int value);
    void setFloat(const char *key, float value);
    const char *get(const char *key) const;
    int getInt(const char *key) const;
#ifdef SAMSUNG_CAMERA_HARDWARE
    int getInt64(const char *key) const;
#endif
    float getFloat(const char *key) const;

    void remove(const char *key);

    void setPreviewSize(int width, int height);
    void getPreviewSize(int *width, int *height) const;
    void getSupportedPreviewSizes(Vector<Size> &sizes) const;

    // Set the dimensions in pixels to the given width and height
    // for video frames. The given width and height must be one
    // of the supported dimensions returned from
    // getSupportedVideoSizes(). Must not be called if
    // getSupportedVideoSizes() returns an empty Vector of Size.
    void setVideoSize(int width, int height);
    // Retrieve the current dimensions (width and height)
    // in pixels for video frames, which must be one of the
    // supported dimensions returned from getSupportedVideoSizes().
    // Must not be called if getSupportedVideoSizes() returns an
    // empty Vector of Size.
    void getVideoSize(int *width, int *height) const;
    // Retrieve a Vector of supported dimensions (width and height)
    // in pixels for video frames. If sizes returned from the method
    // is empty, the camera does not support calls to setVideoSize()
    // or getVideoSize(). In adddition, it also indicates that
    // the camera only has a single output, and does not have
    // separate output for video frames and preview frame.
    void getSupportedVideoSizes(Vector<Size> &sizes) const;
    // Retrieve the preferred preview size (width and height) in pixels
    // for video recording. The given width and height must be one of
    // supported preview sizes returned from getSupportedPreviewSizes().
    // Must not be called if getSupportedVideoSizes() returns an empty
    // Vector of Size. If getSupportedVideoSizes() returns an empty
    // Vector of Size, the width and height returned from this method
    // is invalid, and is "-1x-1".
    void getPreferredPreviewSizeForVideo(int *width, int *height) const;

    void setPreviewFrameRate(int fps);
    int getPreviewFrameRate() const;
    void getPreviewFpsRange(int *min_fps, int *max_fps) const;
#ifdef QCOM_HARDWARE
    void setPreviewFrameRateMode(const char *mode);
    const char *getPreviewFrameRateMode() const;
#endif
    void setPreviewFormat(const char *format);
    const char *getPreviewFormat() const;
    void setPictureSize(int width, int height);
    void getPictureSize(int *width, int *height) const;
    void getSupportedPictureSizes(Vector<Size> &sizes) const;
    void setPictureFormat(const char *format);
    const char *getPictureFormat() const;
#ifdef QCOM_HARDWARE
    void setTouchIndexAec(int x, int y);
    void getTouchIndexAec(int *x, int *y) const;
    void setTouchIndexAf(int x, int y);
    void getTouchIndexAf(int *x, int *y) const;
#endif

    void getMeteringAreaCenter(int * x, int *y) const;

    void dump() const;
    status_t dump(int fd, const Vector<String16>& args) const;

    // Parameter keys to communicate between camera application and driver.
    // The access (read/write, read only, or write only) is viewed from the
    // perspective of applications, not driver.

    // Preview frame size in pixels (width x height).
    // Example value: "480x320". Read/Write.
    static const char KEY_PREVIEW_SIZE[];
    // Supported preview frame sizes in pixels.
    // Example value: "800x600,480x320". Read only.
    static const char KEY_SUPPORTED_PREVIEW_SIZES[];
#ifdef QCOM_HARDWARE
    // Supported PREVIEW/RECORDING SIZES IN HIGH FRAME RATE recording, sizes in pixels.
    // Example value: "800x480,432x320". Read only.
    static const char KEY_SUPPORTED_HFR_SIZES[];
#endif
    // The current minimum and maximum preview fps. This controls the rate of
    // preview frames received (CAMERA_MSG_PREVIEW_FRAME). The minimum and
    // maximum fps must be one of the elements from
    // KEY_SUPPORTED_PREVIEW_FPS_RANGE parameter.
    // Example value: "10500,26623"
    static const char KEY_PREVIEW_FPS_RANGE[];
    // The supported preview fps (frame-per-second) ranges. Each range contains
    // a minimum fps and maximum fps. If minimum fps equals to maximum fps, the
    // camera outputs frames in fixed frame rate. If not, the camera outputs
    // frames in auto frame rate. The actual frame rate fluctuates between the
    // minimum and the maximum. The list has at least one element. The list is
    // sorted from small to large (first by maximum fps and then minimum fps).
    // Example value: "(10500,26623),(15000,26623),(30000,30000)"
    static const char KEY_SUPPORTED_PREVIEW_FPS_RANGE[];
    // The image format for preview frames. See CAMERA_MSG_PREVIEW_FRAME in
    // frameworks/av/include/camera/Camera.h. The default is
    // PIXEL_FORMAT_YUV420SP. Example value: "yuv420sp" or PIXEL_FORMAT_XXX
    // constants. Read/write.
    static const char KEY_PREVIEW_FORMAT[];
    // Supported image formats for preview frames.
    // Example value: "yuv420sp,yuv422i-yuyv". Read only.
    static const char KEY_SUPPORTED_PREVIEW_FORMATS[];
    // Number of preview frames per second. This is the target frame rate. The
    // actual frame rate depends on the driver.
    // Example value: "15". Read/write.
    static const char KEY_PREVIEW_FRAME_RATE[];
    // Supported number of preview frames per second.
    // Example value: "24,15,10". Read.
    static const char KEY_SUPPORTED_PREVIEW_FRAME_RATES[];
#ifdef QCOM_HARDWARE
    // The mode of preview frame rate.
    // Example value: "frame-rate-auto, frame-rate-fixed".
    static const char KEY_PREVIEW_FRAME_RATE_MODE[];
    static const char KEY_SUPPORTED_PREVIEW_FRAME_RATE_MODES[];
    static const char KEY_PREVIEW_FRAME_RATE_AUTO_MODE[];
    static const char KEY_PREVIEW_FRAME_RATE_FIXED_MODE[];
#ifdef QCOM_LEGACY_CAM_PARAMS
    static const char KEY_CAPTURE_MODE[];
    static const char KEY_SUPPORTED_CAPTURE_MODES[];
    static const char KEY_PICTURE_COUNT[];
    static const char KEY_MAX_BURST_PICTURE_COUNT[];
    static const char KEY_SUPPORTED_CONTINUOUS_AF[];
    static const char KEY_SUPPORTED_CAF[];
    static const char CAPTURE_MODE_NORMAL[];
    static const char CAPTURE_MODE_BURST[];
    static const char CAPTURE_MODE_CONTI_BURST[];
    static const char CAPTURE_MODE_HDR[];
    static const char CAPTURE_MODE_HJR[];
    static const char CAPTURE_MODE_PANORAMA[];
#endif
#endif
    // The dimensions for captured pictures in pixels (width x height).
    // Example value: "1024x768". Read/write.
    static const char KEY_PICTURE_SIZE[];
    // Supported dimensions for captured pictures in pixels.
    // Example value: "2048x1536,1024x768". Read only.
    static const char KEY_SUPPORTED_PICTURE_SIZES[];
    // The image format for captured pictures. See CAMERA_MSG_COMPRESSED_IMAGE
    // in frameworks/base/include/camera/Camera.h.
    // Example value: "jpeg" or PIXEL_FORMAT_XXX constants. Read/write.
    static const char KEY_PICTURE_FORMAT[];
    // Supported image formats for captured pictures.
    // Example value: "jpeg,rgb565". Read only.
    static const char KEY_SUPPORTED_PICTURE_FORMATS[];
    // The width (in pixels) of EXIF thumbnail in Jpeg picture.
    // Example value: "512". Read/write.
    static const char KEY_JPEG_THUMBNAIL_WIDTH[];
    // The height (in pixels) of EXIF thumbnail in Jpeg picture.
    // Example value: "384". Read/write.
    static const char KEY_JPEG_THUMBNAIL_HEIGHT[];
    // Supported EXIF thumbnail sizes (width x height). 0x0 means not thumbnail
    // in EXIF.
    // Example value: "512x384,320x240,0x0". Read only.
    static const char KEY_SUPPORTED_JPEG_THUMBNAIL_SIZES[];
    // The quality of the EXIF thumbnail in Jpeg picture. The range is 1 to 100,
    // with 100 being the best.
    // Example value: "90". Read/write.
    static const char KEY_JPEG_THUMBNAIL_QUALITY[];
    // Jpeg quality of captured picture. The range is 1 to 100, with 100 being
    // the best.
    // Example value: "90". Read/write.
    static const char KEY_JPEG_QUALITY[];
    // The rotation angle in degrees relative to the orientation of the camera.
    // This affects the pictures returned from CAMERA_MSG_COMPRESSED_IMAGE. The
    // camera driver may set orientation in the EXIF header without rotating the
    // picture. Or the driver may rotate the picture and the EXIF thumbnail. If
    // the Jpeg picture is rotated, the orientation in the EXIF header will be
    // missing or 1 (row #0 is top and column #0 is left side).
    //
    // Note that the JPEG pictures of front-facing cameras are not mirrored
    // as in preview display.
    //
    // For example, suppose the natural orientation of the device is portrait.
    // The device is rotated 270 degrees clockwise, so the device orientation is
    // 270. Suppose a back-facing camera sensor is mounted in landscape and the
    // top side of the camera sensor is aligned with the right edge of the
    // display in natural orientation. So the camera orientation is 90. The
    // rotation should be set to 0 (270 + 90).
    //
    // Example value: "0" or "90" or "180" or "270". Write only.
    static const char KEY_ROTATION[];
    // GPS latitude coordinate. GPSLatitude and GPSLatitudeRef will be stored in
    // JPEG EXIF header.
    // Example value: "25.032146" or "-33.462809". Write only.
    static const char KEY_GPS_LATITUDE[];
    // GPS longitude coordinate. GPSLongitude and GPSLongitudeRef will be stored
    // in JPEG EXIF header.
    // Example value: "121.564448" or "-70.660286". Write only.
    static const char KEY_GPS_LONGITUDE[];
    // GPS altitude. GPSAltitude and GPSAltitudeRef will be stored in JPEG EXIF
    // header.
    // Example value: "21.0" or "-5". Write only.
    static const char KEY_GPS_ALTITUDE[];

#ifdef QCOM_HARDWARE
    static const char KEY_SKIN_TONE_ENHANCEMENT[] ;
    static const char KEY_SUPPORTED_SKIN_TONE_ENHANCEMENT_MODES[] ;
#endif

    // GPS timestamp (UTC in seconds since January 1, 1970). This should be
    // stored in JPEG EXIF header.
    // Example value: "1251192757". Write only.
    static const char KEY_GPS_TIMESTAMP[];
    // GPS Processing Method
    // Example value: "GPS" or "NETWORK". Write only.
    static const char KEY_GPS_PROCESSING_METHOD[];
    // Current white balance setting.
    // Example value: "auto" or WHITE_BALANCE_XXX constants. Read/write.
    static const char KEY_WHITE_BALANCE[];
    // Supported white balance settings.
    // Example value: "auto,incandescent,daylight". Read only.
    static const char KEY_SUPPORTED_WHITE_BALANCE[];
    // Current color effect setting.
    // Example value: "none" or EFFECT_XXX constants. Read/write.
    static const char KEY_EFFECT[];
    // Supported color effect settings.
    // Example value: "none,mono,sepia". Read only.
    static const char KEY_SUPPORTED_EFFECTS[];
#ifdef QCOM_HARDWARE
    //Touch Af/AEC settings.
    static const char KEY_TOUCH_AF_AEC[];
    static const char KEY_SUPPORTED_TOUCH_AF_AEC[];
    //Touch Index for AEC.
    static const char KEY_TOUCH_INDEX_AEC[];
    //Touch Index for AF.
    static const char KEY_TOUCH_INDEX_AF[];
#endif
    // Current antibanding setting.
    // Example value: "auto" or ANTIBANDING_XXX constants. Read/write.
    static const char KEY_ANTIBANDING[];
    // Supported antibanding settings.
    // Example value: "auto,50hz,60hz,off". Read only.
    static const char KEY_SUPPORTED_ANTIBANDING[];
    // Current scene mode.
    // Example value: "auto" or SCENE_MODE_XXX constants. Read/write.
    static const char KEY_SCENE_MODE[];
    // Supported scene mode settings.
    // Example value: "auto,night,fireworks". Read only.
    static const char KEY_SUPPORTED_SCENE_MODES[];
#ifdef QCOM_HARDWARE
    // Current auto scene detection mode.
    // Example value: "off" or SCENE_DETECT_XXX constants. Read/write.
    static const char KEY_SCENE_DETECT[];
    // Supported auto scene detection settings.
    // Example value: "off,backlight,snow/cloudy". Read only.
    static const char KEY_SUPPORTED_SCENE_DETECT[];
#endif
    // Current flash mode.
    // Example value: "auto" or FLASH_MODE_XXX constants. Read/write.
    static const char KEY_FLASH_MODE[];
    // Supported flash modes.
    // Example value: "auto,on,off". Read only.
    static const char KEY_SUPPORTED_FLASH_MODES[];
    // Current focus mode. This will not be empty. Applications should call
    // CameraHardwareInterface.autoFocus to start the focus if focus mode is
    // FOCUS_MODE_AUTO or FOCUS_MODE_MACRO.
    // Example value: "auto" or FOCUS_MODE_XXX constants. Read/write.
    static const char KEY_FOCUS_MODE[];
    // Supported focus modes.
    // Example value: "auto,macro,fixed". Read only.
    static const char KEY_SUPPORTED_FOCUS_MODES[];
    // The maximum number of focus areas supported. This is the maximum length
    // of KEY_FOCUS_AREAS.
    // Example value: "0" or "2". Read only.
    static const char KEY_MAX_NUM_FOCUS_AREAS[];
    // Current focus areas.
    //
    // Before accessing this parameter, apps should check
    // KEY_MAX_NUM_FOCUS_AREAS first to know the maximum number of focus areas
    // first. If the value is 0, focus area is not supported.
    //
    // Each focus area is a five-element int array. The first four elements are
    // the rectangle of the area (left, top, right, bottom). The direction is
    // relative to the sensor orientation, that is, what the sensor sees. The
    // direction is not affected by the rotation or mirroring of
    // CAMERA_CMD_SET_DISPLAY_ORIENTATION. Coordinates range from -1000 to 1000.
    // (-1000,-1000) is the upper left point. (1000, 1000) is the lower right
    // point. The width and height of focus areas cannot be 0 or negative.
    //
    // The fifth element is the weight. Values for weight must range from 1 to
    // 1000.  The weight should be interpreted as a per-pixel weight - all
    // pixels in the area have the specified weight. This means a small area
    // with the same weight as a larger area will have less influence on the
    // focusing than the larger area. Focus areas can partially overlap and the
    // driver will add the weights in the overlap region.
    //
    // A special case of single focus area (0,0,0,0,0) means driver to decide
    // the focus area. For example, the driver may use more signals to decide
    // focus areas and change them dynamically. Apps can set (0,0,0,0,0) if they
    // want the driver to decide focus areas.
    //
    // Focus areas are relative to the current field of view (KEY_ZOOM). No
    // matter what the zoom level is, (-1000,-1000) represents the top of the
    // currently visible camera frame. The focus area cannot be set to be
    // outside the current field of view, even when using zoom.
    //
    // Focus area only has effect if the current focus mode is FOCUS_MODE_AUTO,
    // FOCUS_MODE_MACRO, FOCUS_MODE_CONTINUOUS_VIDEO, or
    // FOCUS_MODE_CONTINUOUS_PICTURE.
    // Example value: "(-10,-10,0,0,300),(0,0,10,10,700)". Read/write.
    static const char KEY_FOCUS_AREAS[];
    // Focal length in millimeter.
    // Example value: "4.31". Read only.
    static const char KEY_FOCAL_LENGTH[];
    // Horizontal angle of view in degrees.
    // Example value: "54.8". Read only.
    static const char KEY_HORIZONTAL_VIEW_ANGLE[];
    // Vertical angle of view in degrees.
    // Example value: "42.5". Read only.
    static const char KEY_VERTICAL_VIEW_ANGLE[];
    // Exposure compensation index. 0 means exposure is not adjusted.
    // Example value: "-5" or "5". Read/write.
    static const char KEY_EXPOSURE_COMPENSATION[];
    // The maximum exposure compensation index (>=0).
    // Example value: "6". Read only.
    static const char KEY_MAX_EXPOSURE_COMPENSATION[];
    // The minimum exposure compensation index (<=0).
    // Example value: "-6". Read only.
    static const char KEY_MIN_EXPOSURE_COMPENSATION[];
    // The exposure compensation step. Exposure compensation index multiply by
    // step eqals to EV. Ex: if exposure compensation index is -6 and step is
    // 0.3333, EV is -2.
    // Example value: "0.333333333" or "0.5". Read only.
    static const char KEY_EXPOSURE_COMPENSATION_STEP[];
    // The state of the auto-exposure lock. "true" means that
    // auto-exposure is locked to its current value and will not
    // change. "false" means the auto-exposure routine is free to
    // change exposure values. If auto-exposure is already locked,
    // setting this to true again has no effect (the driver will not
    // recalculate exposure values). Changing exposure compensation
    // settings will still affect the exposure settings while
    // auto-exposure is locked. Stopping preview or taking a still
    // image will not change the lock. In conjunction with
    // exposure compensation, this allows for capturing multi-exposure
    // brackets with known relative exposure values. Locking
    // auto-exposure after open but before the first call to
    // startPreview may result in severely over- or under-exposed
    // images.  The driver will not change the AE lock after
    // auto-focus completes.
    static const char KEY_AUTO_EXPOSURE_LOCK[];
    // Whether locking the auto-exposure is supported. "true" means it is, and
    // "false" or this key not existing means it is not supported.
    static const char KEY_AUTO_EXPOSURE_LOCK_SUPPORTED[];
    // The state of the auto-white balance lock. "true" means that
    // auto-white balance is locked to its current value and will not
    // change. "false" means the auto-white balance routine is free to
    // change white balance values. If auto-white balance is already
    // locked, setting this to true again has no effect (the driver
    // will not recalculate white balance values). Stopping preview or
    // taking a still image will not change the lock. In conjunction
    // with exposure compensation, this allows for capturing
    // multi-exposure brackets with fixed white balance. Locking
    // auto-white balance after open but before the first call to
    // startPreview may result in severely incorrect color.  The
    // driver will not change the AWB lock after auto-focus
    // completes.
    static const char KEY_AUTO_WHITEBALANCE_LOCK[];
    // Whether locking the auto-white balance is supported. "true"
    // means it is, and "false" or this key not existing means it is
    // not supported.
    static const char KEY_AUTO_WHITEBALANCE_LOCK_SUPPORTED[];

    // The maximum number of metering areas supported. This is the maximum
    // length of KEY_METERING_AREAS.
    // Example value: "0" or "2". Read only.
    static const char KEY_MAX_NUM_METERING_AREAS[];
    // Current metering areas. Camera driver uses these areas to decide
    // exposure.
    //
    // Before accessing this parameter, apps should check
    // KEY_MAX_NUM_METERING_AREAS first to know the maximum number of metering
    // areas first. If the value is 0, metering area is not supported.
    //
    // Each metering area is a rectangle with specified weight. The direction is
    // relative to the sensor orientation, that is, what the sensor sees. The
    // direction is not affected by the rotation or mirroring of
    // CAMERA_CMD_SET_DISPLAY_ORIENTATION. Coordinates of the rectangle range
    // from -1000 to 1000. (-1000, -1000) is the upper left point. (1000, 1000)
    // is the lower right point. The width and height of metering areas cannot
    // be 0 or negative.
    //
    // The fifth element is the weight. Values for weight must range from 1 to
    // 1000.  The weight should be interpreted as a per-pixel weight - all
    // pixels in the area have the specified weight. This means a small area
    // with the same weight as a larger area will have less influence on the
    // metering than the larger area. Metering areas can partially overlap and
    // the driver will add the weights in the overlap region.
    //
    // A special case of all-zero single metering area means driver to decide
    // the metering area. For example, the driver may use more signals to decide
    // metering areas and change them dynamically. Apps can set all-zero if they
    // want the driver to decide metering areas.
    //
    // Metering areas are relative to the current field of view (KEY_ZOOM).
    // No matter what the zoom level is, (-1000,-1000) represents the top of the
    // currently visible camera frame. The metering area cannot be set to be
    // outside the current field of view, even when using zoom.
    //
    // No matter what metering areas are, the final exposure are compensated
    // by KEY_EXPOSURE_COMPENSATION.
    // Example value: "(-10,-10,0,0,300),(0,0,10,10,700)". Read/write.
    static const char KEY_METERING_AREAS[];
    // Current zoom value.
    // Example value: "0" or "6". Read/write.
    static const char KEY_ZOOM[];
    // Maximum zoom value.
    // Example value: "6". Read only.
    static const char KEY_MAX_ZOOM[];
    // The zoom ratios of all zoom values. The zoom ratio is in 1/100
    // increments. Ex: a zoom of 3.2x is returned as 320. The number of list
    // elements is KEY_MAX_ZOOM + 1. The first element is always 100. The last
    // element is the zoom ratio of zoom value KEY_MAX_ZOOM.
    // Example value: "100,150,200,250,300,350,400". Read only.
    static const char KEY_ZOOM_RATIOS[];
    // Whether zoom is supported. Zoom is supported if the value is "true". Zoom
    // is not supported if the value is not "true" or the key does not exist.
    // Example value: "true". Read only.
    static const char KEY_ZOOM_SUPPORTED[];
    // Whether if smooth zoom is supported. Smooth zoom is supported if the
    // value is "true". It is not supported if the value is not "true" or the
    // key does not exist.
    // See CAMERA_CMD_START_SMOOTH_ZOOM, CAMERA_CMD_STOP_SMOOTH_ZOOM, and
    // CAMERA_MSG_ZOOM in frameworks/base/include/camera/Camera.h.
    // Example value: "true". Read only.
    static const char KEY_SMOOTH_ZOOM_SUPPORTED[];

    // The distances (in meters) from the camera to where an object appears to
    // be in focus. The object is sharpest at the optimal focus distance. The
    // depth of field is the far focus distance minus near focus distance.
    //
    // Focus distances may change after starting auto focus, canceling auto
    // focus, or starting the preview. Applications can read this anytime to get
    // the latest focus distances. If the focus mode is FOCUS_MODE_CONTINUOUS,
    // focus distances may change from time to time.
    //
    // This is intended to estimate the distance between the camera and the
    // subject. After autofocus, the subject distance may be within near and far
    // focus distance. However, the precision depends on the camera hardware,
    // autofocus algorithm, the focus area, and the scene. The error can be
    // large and it should be only used as a reference.
    //
    // Far focus distance > optimal focus distance > near focus distance. If
    // the far focus distance is infinity, the value should be "Infinity" (case
    // sensitive). The format is three float values separated by commas. The
    // first is near focus distance. The second is optimal focus distance. The
    // third is far focus distance.
    // Example value: "0.95,1.9,Infinity" or "0.049,0.05,0.051". Read only.
    static const char KEY_FOCUS_DISTANCES[];

    // The current dimensions in pixels (width x height) for video frames.
    // The width and height must be one of the supported sizes retrieved
    // via KEY_SUPPORTED_VIDEO_SIZES.
    // Example value: "1280x720". Read/write.
    static const char KEY_VIDEO_SIZE[];
    // A list of the supported dimensions in pixels (width x height)
    // for video frames. See CAMERA_MSG_VIDEO_FRAME for details in
    // frameworks/base/include/camera/Camera.h.
    // Example: "176x144,1280x720". Read only.
    static const char KEY_SUPPORTED_VIDEO_SIZES[];

    // The maximum number of detected faces supported by hardware face
    // detection. If the value is 0, hardware face detection is not supported.
    // Example: "5". Read only
    static const char KEY_MAX_NUM_DETECTED_FACES_HW[];

    // The maximum number of detected faces supported by software face
    // detection. If the value is 0, software face detection is not supported.
    // Example: "5". Read only
    static const char KEY_MAX_NUM_DETECTED_FACES_SW[];

    // Preferred preview frame size in pixels for video recording.
    // The width and height must be one of the supported sizes retrieved
    // via KEY_SUPPORTED_PREVIEW_SIZES. This key can be used only when
    // getSupportedVideoSizes() does not return an empty Vector of Size.
    // Camcorder applications are recommended to set the preview size
    // to a value that is not larger than the preferred preview size.
    // In other words, the product of the width and height of the
    // preview size should not be larger than that of the preferred
    // preview size. In addition, we recommend to choos a preview size
    // that has the same aspect ratio as the resolution of video to be
    // recorded.
    // Example value: "800x600". Read only.
    static const char KEY_PREFERRED_PREVIEW_SIZE_FOR_VIDEO[];

    // The image format for video frames. See CAMERA_MSG_VIDEO_FRAME in
    // frameworks/base/include/camera/Camera.h.
    // Example value: "yuv420sp" or PIXEL_FORMAT_XXX constants. Read only.
    static const char KEY_VIDEO_FRAME_FORMAT[];

    // Sets the hint of the recording mode. If this is true, MediaRecorder.start
    // may be faster or has less glitches. This should be called before starting
    // the preview for the best result. But it is allowed to change the hint
    // while the preview is active. The default value is false.
    //
    // The apps can still call Camera.takePicture when the hint is true. The
    // apps can call MediaRecorder.start when the hint is false. But the
    // performance may be worse.
    // Example value: "true" or "false". Read/write.
    static const char KEY_RECORDING_HINT[];

    // Returns true if video snapshot is supported. That is, applications
    // can call Camera.takePicture during recording. Applications do not need to
    // call Camera.startPreview after taking a picture. The preview will be
    // still active. Other than that, taking a picture during recording is
    // identical to taking a picture normally. All settings and methods related
    // to takePicture work identically. Ex: KEY_PICTURE_SIZE,
    // KEY_SUPPORTED_PICTURE_SIZES, KEY_JPEG_QUALITY, KEY_ROTATION, and etc.
    // The picture will have an EXIF header. FLASH_MODE_AUTO and FLASH_MODE_ON
    // also still work, but the video will record the flash.
    //
    // Applications can set shutter callback as null to avoid the shutter
    // sound. It is also recommended to set raw picture and post view callbacks
    // to null to avoid the interrupt of preview display.
    //
    // Field-of-view of the recorded video may be different from that of the
    // captured pictures.
    // Example value: "true" or "false". Read only.
    static const char KEY_VIDEO_SNAPSHOT_SUPPORTED[];
    static const char KEY_FULL_VIDEO_SNAP_SUPPORTED[];

#ifdef QCOM_HARDWARE
#ifndef HAVE_ISO
    static const char KEY_ISO_MODE[];
    static const char KEY_SUPPORTED_ISO_MODES[];
#endif
    static const char KEY_LENSSHADE[] ;
    static const char KEY_SUPPORTED_LENSSHADE_MODES[] ;

    static const char KEY_AUTO_EXPOSURE[];
    static const char KEY_SUPPORTED_AUTO_EXPOSURE[];

    static const char KEY_GPS_LATITUDE_REF[];
    static const char KEY_GPS_LONGITUDE_REF[];
    static const char KEY_GPS_ALTITUDE_REF[];
    static const char KEY_GPS_STATUS[];
    static const char KEY_EXIF_DATETIME[];
#endif

    // The state of the video stabilization. If set to true, both the
    // preview stream and the recorded video stream are stabilized by
    // the camera. Only valid to set if KEY_VIDEO_STABILIZATION_SUPPORTED is
    // set to true.
    //
    // The value of this key can be changed any time the camera is
    // open. If preview or recording is active, it is acceptable for
    // there to be a slight video glitch when video stabilization is
    // toggled on and off.
    //
    // This only stabilizes video streams (between-frames stabilization), and
    // has no effect on still image capture.
    static const char KEY_VIDEO_STABILIZATION[];

#ifdef QCOM_HARDWARE
    static const char KEY_MEMORY_COLOR_ENHANCEMENT[];
    static const char KEY_SUPPORTED_MEM_COLOR_ENHANCE_MODES[];

    static const char KEY_POWER_MODE_SUPPORTED[];
    static const char KEY_POWER_MODE[];

    static const char KEY_ZSL[];
    static const char KEY_SUPPORTED_ZSL_MODES[];

    static const char KEY_CAMERA_MODE[];

    static const char KEY_VIDEO_HIGH_FRAME_RATE[];
    static const char KEY_SUPPORTED_VIDEO_HIGH_FRAME_RATE_MODES[];
    static const char KEY_HIGH_DYNAMIC_RANGE_IMAGING[];
    static const char KEY_SUPPORTED_HDR_IMAGING_MODES[];
#endif
    // Returns true if video stabilization is supported. That is, applications
    // can set KEY_VIDEO_STABILIZATION to true and have a stabilized preview
    // stream and record stabilized videos.
    static const char KEY_VIDEO_STABILIZATION_SUPPORTED[];

    // Supported modes for special effects with light.
    // Example values: "lowlight,hdr".
    static const char KEY_LIGHTFX[];

#ifdef HAVE_ISO
    static const char KEY_SUPPORTED_ISO_MODES[];
    static const char KEY_ISO_MODE[];
#endif

#ifdef SAMSUNG_CAMERA_HARDWARE
    static const char KEY_ANTI_SHAKE_MODE[];
    static const char KEY_METERING[];
    static const char KEY_WDR[];
    static const char KEY_WEATHER[];
    static const char KEY_CITYID[];
#endif

#ifdef HTC_CAMERA_HARDWARE
    static const char KEY_TIME_CONS_POST_PROCESSING[];
    static const char KEY_OIS_MODE[];
    static const char KEY_APP_OIS_SETTING[];
    static const char KEY_OIS_SUPPORT[];
    static const char KEY_CONTIBURST_TYPE[];
    static const char KEY_CAPTURE_MODE[];
    static const char CAPTURE_MODE_NORMAL[];
    static const char CAPTURE_MODE_CONTI_ZOE[];
    static const char CAPTURE_MODE_CONTI_BURST[];
    static const char CAPTURE_MODE_CONTI_BURST_ONE_SHOT[];
    static const char CAPTURE_MODE_HDR[];
    static const char CAPTURE_MODE_PANORAMA[];
    static const char CAPTURE_MODE_ZOE[];
    static const char CAPTURE_MODE_EIS[];
    static const char KEY_CONTI_BURST_STATE[];
    static const char KEY_SUPPORTED_CAPTURE_MODES[];
    static const char KEY_MIN_CONTRAST[];
    static const char KEY_DEF_CONTRAST[];
    static const char KEY_MIN_SHARPNESS[];
    static const char KEY_DEF_SHARPNESS[];
    static const char KEY_MIN_SATURATION[];
    static const char KEY_DEF_SATURATION[];
    static const char KEY_SINGLE_ISP_OUTPUT_ENABLED[];
    static const char POST_PROCESSING_ENABLE[];
    static const char POST_PROCESSING_BYPASS[];
    static const char POST_PROCESSING_DELAY[];
    static const char SCENE_MODE_OFF[];
    static const char SCENE_MODE_TEXT[];
    static const char BURST_MODE_LIMIT20[];
    static const char BURST_MODE_UNLIMITED[];
    static const char OIS_MODE_OFF[];
    static const char OIS_MODE_ON[];
    static const char CONTI_BURST_CAPTURING[];
    static const char CONTI_BURST_CAPTURE_DONE[];
    static const char APP_OIS_SETTING_FALSE[];
    static const char APP_OIS_SETTING_TRUE[];
    static const char KEY_GPU_EFFECT[];
    static const char KEY_GPU_EFFECT_PARAM_0[];
    static const char KEY_GPU_EFFECT_PARAM_1[];
    static const char KEY_GPU_EFFECT_PARAM_2[];
    static const char KEY_GPU_EFFECT_PARAM_3[];
    static const char KEY_FORCE_USE_AUDIO_ENABLED[];
#endif

#ifdef LG_CAMERA_HARDWARE
    static const char AUDIO_ZOOM_OFF[];
    static const char AUDIO_ZOOM_ON[];
    static const char BEAUTY_SHOT_OFF[];
    static const char BEAUTY_SHOT_ON[];
    static const char BURST_SHOT_OFF[];
    static const char BURST_SHOT_ON[];
    static const char KEY_AUDIO_ZOOM[];
    static const char KEY_AUDIO_ZOOM_SUPPORTED[];
    static const char KEY_BEAUTY_SHOT[];
    static const char KEY_BEAUTY_SHOT_SUPPORTED[];
    static const char KEY_BURST_SHOT[];
    static const char KEY_BURST_SHOT_SUPPORTED[];
    static const char KEY_FOCUS_MODE_OBJECT_TRACKING[];
    static const char KEY_FOCUS_MODE_OBJECT_TRACKING_SUPPORTED[];
    static const char KEY_VIDEO_WDR[];
    static const char KEY_VIDEO_WDR_SUPPORTED[];
    static const char VIDEO_WDR_OFF[];
    static const char VIDEO_WDR_ON[];
    static const char OBJECT_TRACKING_ON[];
    static const char OBJECT_TRACKING_OFF[];
#endif

    static const char KEY_AE_BRACKET_HDR[];

    // Value for KEY_ZOOM_SUPPORTED or KEY_SMOOTH_ZOOM_SUPPORTED.
    static const char TRUE[];
    static const char FALSE[];

    // Value for KEY_FOCUS_DISTANCES.
    static const char FOCUS_DISTANCE_INFINITY[];

#ifdef SONY_CAM_PARAMS
    static const char KEY_SONY_ISO[];
    static const char KEY_SONY_METERING_MODE[];
    static const char KEY_SONY_VS[];
    static const char KEY_SONY_MAX_BURST_SHOT_SIZE[];
    static const char KEY_SONY_BURST_SHOT_VALUES[];
    static const char KEY_SONY_VS_VALUES[];
    static const char KEY_SONY_METERING_MODE_VALUES[];
    static const char KEY_SONY_AE_MODE[];
    static const char KEY_SONY_AE_MODE_VALUES[];
    static const char KEY_SONY_ISO_VALUES[];
    static const char KEY_SONY_IS[];
    static const char KEY_SONY_IS_VALUES[];
    static const char KEY_SONY_EXTENSION_VERSION[];
    static const char KEY_SONY_FOCUS_AREA[];
    static const char KEY_SONY_FOCUS_AREA_VALUES[];
    static const char KEY_SONY_MAX_MULTI_FOCUS_NUM[];
    static const char KEY_SONY_SCENE_DETECT_SUPPORTED[];
#endif

#ifdef QCOM_HARDWARE
    // DENOISE
    static const char KEY_DENOISE[];
    static const char KEY_SUPPORTED_DENOISE[];

    //Selectable zone AF.
    static const char KEY_SELECTABLE_ZONE_AF[];
    static const char KEY_SUPPORTED_SELECTABLE_ZONE_AF[];

    //Face Detection
    static const char KEY_FACE_DETECTION[];
    static const char KEY_SUPPORTED_FACE_DETECTION[];

    //Redeye Reduction
    static const char KEY_REDEYE_REDUCTION[];
    static const char KEY_SUPPORTED_REDEYE_REDUCTION[];
#endif

    // Values for white balance settings.
    static const char WHITE_BALANCE_AUTO[];
    static const char WHITE_BALANCE_INCANDESCENT[];
    static const char WHITE_BALANCE_FLUORESCENT[];
    static const char WHITE_BALANCE_WARM_FLUORESCENT[];
    static const char WHITE_BALANCE_DAYLIGHT[];
    static const char WHITE_BALANCE_CLOUDY_DAYLIGHT[];
    static const char WHITE_BALANCE_TWILIGHT[];
    static const char WHITE_BALANCE_SHADE[];
#ifdef OPPO_CAMERA_HARDWARE
    static const char WHITE_BALANCE_MANUAL_CCT[];
#endif

    // Values for effect settings.
    static const char EFFECT_NONE[];
    static const char EFFECT_MONO[];
    static const char EFFECT_NEGATIVE[];
    static const char EFFECT_SOLARIZE[];
    static const char EFFECT_SEPIA[];
    static const char EFFECT_POSTERIZE[];
    static const char EFFECT_WHITEBOARD[];
    static const char EFFECT_BLACKBOARD[];
    static const char EFFECT_AQUA[];
#ifdef SAMSUNG_CAMERA_HARDWARE
    static const char EFFECT_CARTOONIZE[];
    static const char EFFECT_POINT_RED_YELLOW[];
    static const char EFFECT_POINT_GREEN[];
    static const char EFFECT_POINT_BLUE[];
    static const char EFFECT_VINTAGE_COLD[];
    static const char EFFECT_VINTAGE_WARM[];
    static const char EFFECT_WASHED[];
#endif
#ifdef QCOM_HARDWARE
    static const char EFFECT_EMBOSS[];
    static const char EFFECT_SKETCH[];
    static const char EFFECT_NEON[];

    // Values for Touch AF/AEC
    static const char TOUCH_AF_AEC_OFF[] ;
    static const char TOUCH_AF_AEC_ON[] ;
#endif

    // Values for antibanding settings.
    static const char ANTIBANDING_AUTO[];
    static const char ANTIBANDING_50HZ[];
    static const char ANTIBANDING_60HZ[];
    static const char ANTIBANDING_OFF[];

    // Values for flash mode settings.
    // Flash will not be fired.
    static const char FLASH_MODE_OFF[];
    // Flash will be fired automatically when required. The flash may be fired
    // during preview, auto-focus, or snapshot depending on the driver.
    static const char FLASH_MODE_AUTO[];
    // Flash will always be fired during snapshot. The flash may also be
    // fired during preview or auto-focus depending on the driver.
    static const char FLASH_MODE_ON[];
    // Flash will be fired in red-eye reduction mode.
    static const char FLASH_MODE_RED_EYE[];
    // Constant emission of light during preview, auto-focus and snapshot.
    // This can also be used for video recording.
    static const char FLASH_MODE_TORCH[];

    // Values for scene mode settings.
    static const char SCENE_MODE_AUTO[];
    static const char SCENE_MODE_ASD[];
    static const char SCENE_MODE_ACTION[];
    static const char SCENE_MODE_PORTRAIT[];
    static const char SCENE_MODE_LANDSCAPE[];
    static const char SCENE_MODE_NIGHT[];
    static const char SCENE_MODE_NIGHT_PORTRAIT[];
    static const char SCENE_MODE_THEATRE[];
    static const char SCENE_MODE_BEACH[];
    static const char SCENE_MODE_SNOW[];
    static const char SCENE_MODE_SUNSET[];
    static const char SCENE_MODE_STEADYPHOTO[];
    static const char SCENE_MODE_FIREWORKS[];
    static const char SCENE_MODE_SPORTS[];
    static const char SCENE_MODE_PARTY[];
    static const char SCENE_MODE_CANDLELIGHT[];
#ifdef QCOM_HARDWARE
    static const char SCENE_MODE_BACKLIGHT[];
    static const char SCENE_MODE_FLOWERS[];
    static const char SCENE_MODE_AR[];
#ifdef QCOM_SONY_HARDWARE
    static const char EX_SCENE_MODE_DOCUMENT[];
    static const char SCENE_MODE_DOCUMENT[];
#endif
#endif
    // Applications are looking for a barcode. Camera driver will be optimized
    // for barcode reading.
    static const char SCENE_MODE_BARCODE[];
    // A high-dynamic range mode. In this mode, the HAL module will use a
    // capture strategy that extends the dynamic range of the captured
    // image in some fashion. Only the final image is returned.
    static const char SCENE_MODE_HDR[];

    // Pixel color formats for KEY_PREVIEW_FORMAT, KEY_PICTURE_FORMAT,
    // and KEY_VIDEO_FRAME_FORMAT
#ifdef QCOM_HARDWARE
    static const char SCENE_DETECT_OFF[];
    static const char SCENE_DETECT_ON[];
#endif
    static const char PIXEL_FORMAT_YUV422SP[];
    static const char PIXEL_FORMAT_YUV420SP[]; // NV21
#ifdef QCOM_HARDWARE
    static const char PIXEL_FORMAT_YUV420SP_ADRENO[]; // ADRENO
#endif
    static const char PIXEL_FORMAT_YUV422I[]; // YUY2
    static const char PIXEL_FORMAT_YUV420P[]; // YV12
    static const char PIXEL_FORMAT_RGB565[];
    static const char PIXEL_FORMAT_RGBA8888[];
    static const char PIXEL_FORMAT_JPEG[];
    // Raw bayer format used for images, which is 10 bit precision samples
    // stored in 16 bit words. The filter pattern is RGGB.
    static const char PIXEL_FORMAT_BAYER_RGGB[];

    // Pixel format is not known to the framework
    static const char PIXEL_FORMAT_ANDROID_OPAQUE[];

#ifdef QCOM_HARDWARE
    static const char PIXEL_FORMAT_RAW[];
    static const char PIXEL_FORMAT_YV12[]; // NV21
    static const char PIXEL_FORMAT_NV12[]; //NV12
#endif

    // Values for focus mode settings.
    // Auto-focus mode. Applications should call
    // CameraHardwareInterface.autoFocus to start the focus in this mode.
    static const char FOCUS_MODE_AUTO[];
    // Focus is set at infinity. Applications should not call
    // CameraHardwareInterface.autoFocus in this mode.
    static const char FOCUS_MODE_INFINITY[];
    // Macro (close-up) focus mode. Applications should call
    // CameraHardwareInterface.autoFocus to start the focus in this mode.
    static const char FOCUS_MODE_MACRO[];
    // Focus is fixed. The camera is always in this mode if the focus is not
    // adjustable. If the camera has auto-focus, this mode can fix the
    // focus, which is usually at hyperfocal distance. Applications should
    // not call CameraHardwareInterface.autoFocus in this mode.
    static const char FOCUS_MODE_FIXED[];
    // Extended depth of field (EDOF). Focusing is done digitally and
    // continuously. Applications should not call
    // CameraHardwareInterface.autoFocus in this mode.
    static const char FOCUS_MODE_EDOF[];
    // Continuous auto focus mode intended for video recording. The camera
    // continuously tries to focus. This is the best choice for video
    // recording because the focus changes smoothly . Applications still can
    // call CameraHardwareInterface.takePicture in this mode but the subject may
    // not be in focus. Auto focus starts when the parameter is set.
    //
    // Applications can call CameraHardwareInterface.autoFocus in this mode. The
    // focus callback will immediately return with a boolean that indicates
    // whether the focus is sharp or not. The focus position is locked after
    // autoFocus call. If applications want to resume the continuous focus,
    // cancelAutoFocus must be called. Restarting the preview will not resume
    // the continuous autofocus. To stop continuous focus, applications should
    // change the focus mode to other modes.
    static const char FOCUS_MODE_CONTINUOUS_VIDEO[];
    // Continuous auto focus mode intended for taking pictures. The camera
    // continuously tries to focus. The speed of focus change is more aggressive
    // than FOCUS_MODE_CONTINUOUS_VIDEO. Auto focus starts when the parameter is
    // set.
    //
    // Applications can call CameraHardwareInterface.autoFocus in this mode. If
    // the autofocus is in the middle of scanning, the focus callback will
    // return when it completes. If the autofocus is not scanning, focus
    // callback will immediately return with a boolean that indicates whether
    // the focus is sharp or not. The apps can then decide if they want to take
    // a picture immediately or to change the focus mode to auto, and run a full
    // autofocus cycle. The focus position is locked after autoFocus call. If
    // applications want to resume the continuous focus, cancelAutoFocus must be
    // called. Restarting the preview will not resume the continuous autofocus.
    // To stop continuous focus, applications should change the focus mode to
    // other modes.
    static const char FOCUS_MODE_CONTINUOUS_PICTURE[];

#ifdef OPPO_CAMERA_HARDWARE
    static const char FOCUS_MODE_MANUAL_POSITION[];
#endif

    // Values for light special effects
    // Low-light enhancement mode
    static const char LIGHTFX_LOWLIGHT[];
    // High-dynamic range mode
    static const char LIGHTFX_HDR[];

#ifdef QCOM_HARDWARE
#ifdef QCOM_LEGACY_CAM_PARAMS
    static const char FOCUS_MODE_CONTINUOUS_CAMERA[];

    // Values for Continuous AF
    static const char CAF_OFF[] ;
    static const char CAF_ON[] ;
    // Proprietaries from CodeAurora use these...
    static const char CONTINUOUS_AF_OFF[] ;
    static const char CONTINUOUS_AF_ON[] ;
    static const char KEY_CONTINUOUS_AF[] ;
    static const char KEY_CAF[] ;
    static const char KEY_TAKING_PICTURE_ZOOM[];
    static const char KEY_PANORAMA_MODE[];
    static const char PANORAMA_MODE_NOT_INPROGRESS[];
    static const char PANORAMA_MODE_INPROGRESS[];
#endif
    // Normal focus mode. Applications should call
    // CameraHardwareInterface.autoFocus to start the focus in this mode.
    static const char FOCUS_MODE_NORMAL[];
    static const char ISO_AUTO[];
    static const char ISO_HJR[] ;
    static const char ISO_100[];
    static const char ISO_200[] ;
    static const char ISO_400[];
    static const char ISO_800[];
    static const char ISO_1600[];
    static const char ISO_3200[];
    static const char ISO_6400[];
    // Values for Lens Shading
    static const char LENSSHADE_ENABLE[] ;
    static const char LENSSHADE_DISABLE[] ;

    // Values for auto exposure settings.
    static const char AUTO_EXPOSURE_FRAME_AVG[];
    static const char AUTO_EXPOSURE_CENTER_WEIGHTED[];
    static const char AUTO_EXPOSURE_SPOT_METERING[];

    static const char KEY_SHARPNESS[];
    static const char KEY_MAX_SHARPNESS[];
#ifdef QCOM_LEGACY_CAM_PARAMS
    static const char KEY_MIN_SHARPNESS[];
#endif
    static const char KEY_CONTRAST[];
    static const char KEY_MAX_CONTRAST[];
#ifdef QCOM_LEGACY_CAM_PARAMS
    static const char KEY_MIN_CONTRAST[];
#endif
    static const char KEY_SATURATION[];
    static const char KEY_MAX_SATURATION[];
#ifdef QCOM_LEGACY_CAM_PARAMS
    static const char KEY_MIN_SATURATION[];
#endif

    static const char KEY_HISTOGRAM[] ;
    static const char KEY_SUPPORTED_HISTOGRAM_MODES[] ;
    // Values for HISTOGRAM
    static const char HISTOGRAM_ENABLE[] ;
    static const char HISTOGRAM_DISABLE[] ;

    // Values for SKIN TONE ENHANCEMENT
    static const char SKIN_TONE_ENHANCEMENT_ENABLE[] ;
    static const char SKIN_TONE_ENHANCEMENT_DISABLE[] ;

    // Values for Denoise
    static const char DENOISE_OFF[] ;
    static const char DENOISE_ON[] ;

    // Values for auto exposure settings.
    static const char SELECTABLE_ZONE_AF_AUTO[];
    static const char SELECTABLE_ZONE_AF_SPOT_METERING[];
    static const char SELECTABLE_ZONE_AF_CENTER_WEIGHTED[];
    static const char SELECTABLE_ZONE_AF_FRAME_AVERAGE[];

    // Values for Face Detection settings.
    static const char FACE_DETECTION_OFF[];
    static const char FACE_DETECTION_ON[];

    // Values for MCE settings.
    static const char MCE_ENABLE[];
    static const char MCE_DISABLE[];

    // Values for ZSL settings.
    static const char ZSL_OFF[];
    static const char ZSL_ON[];

    // Values for HDR Bracketing settings.
    static const char AE_BRACKET_HDR_OFF[];
    static const char AE_BRACKET_HDR[];
    static const char AE_BRACKET[];

    //POWER MODE
    static const char LOW_POWER[];
    static const char NORMAL_POWER[];

    // Values for HFR settings.
    static const char VIDEO_HFR_OFF[];
    static const char VIDEO_HFR_2X[];
    static const char VIDEO_HFR_3X[];
    static const char VIDEO_HFR_4X[];

    // Values for Redeye Reduction settings.
    static const char REDEYE_REDUCTION_ENABLE[];
    static const char REDEYE_REDUCTION_DISABLE[];
    // Values for HDR settings.
    static const char HDR_ENABLE[];
    static const char HDR_DISABLE[];

#ifdef QCOM_SONY_HARDWARE
     static const char KEY_EX_SUPPORTED_METERING_MODES[];
     static const char KEY_SEMC_METRY_MODE[];
     static const char SEMC_METRY_CENTER[];
     static const char SEMC_METRY_FRAME[];
     static const char SEMC_METRY_SPOT[];
#endif

#if defined(QCOM_HARDWARE) && defined(SAMSUNG_CAMERA_LEGACY)
    static const char FOCUS_MODE_FACEDETECT[];
    static const char FOCUS_MODE_TOUCHAF[];
    static const char ISO_50[];
    static const char KEY_ANTI_SHAKE_MODE[];
    static const char KEY_AUTO_CONTRAST[];
    static const char KEY_BEAUTY_MODE[];
    static const char KEY_BLUR_MODE[];
    static const char KEY_VINTAGE_MODE[];
    static const char KEY_WDR_MODE[];
    static const char VINTAGE_MODE_BNW[];
    static const char VINTAGE_MODE_COOL[];
    static const char VINTAGE_MODE_NORMAL[];
    static const char VINTAGE_MODE_OFF[];
    static const char VINTAGE_MODE_WARM[];
    static const char SCENE_MODE_DAWN[];
    static const char SCENE_MODE_DUSKDAWN[];
    static const char SCENE_MODE_FALL[];
    static const char SCENE_MODE_FALL_COLOR[];
    static const char SCENE_MODE_TEXT[];
#endif

   // Values for Redeye Reduction settings.
   // static const char REDEYE_REDUCTION_ENABLE[];
   // static const char REDEYE_REDUCTION_DISABLE[];
   // Values for HDR settings.
   //    static const char HDR_ENABLE[];
   //    static const char HDR_DISABLE[];

    enum {
        CAMERA_ORIENTATION_UNKNOWN = 0,
        CAMERA_ORIENTATION_PORTRAIT = 1,
        CAMERA_ORIENTATION_LANDSCAPE = 2,
    };
    int getOrientation() const;
    void setOrientation(int orientation);
    void setPreviewFpsRange(int minFPS,int maxFPS);
#ifdef QCOM_LEGACY_CAM_PARAMS
    void setPostviewSize(int x, int y);
#endif
    void getSupportedHfrSizes(Vector<Size> &sizes) const;

#ifdef QCOM_SONY_HARDWARE
    void getFocusAreaCenter(int *x, int *y) const;
#endif
#endif

private:
    DefaultKeyedVector<String8,String8>    mMap;
};

}; // namespace android

#endif
