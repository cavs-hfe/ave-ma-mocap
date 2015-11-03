/*=========================================================
//
// File: EVaRT.h  v100
//
// Created by Ned Phipps, Nov-98
//
// This file defines the interface to EVaRT output.
//
//------------------------------------------------------*/

#ifndef EVaRT_H
#define EVaRT_H

#ifdef _WINDOWS
    #define DLL __declspec(dllexport)
#else
    #define DLL
#endif


// Return codes

#define OK               0             //!< Okay
#define ERRFLAG         -1             //!< General Error
#define API_ERROR       -2             //!< Invalid use of the API
#define NETWORK_ERROR   -3             //!< Network Error
#define FILE_ERROR      -4             //!< File Error


// Array dimensions

#define MAX_SEGMENTS 150
#define MAX_MARKERS  192
#define MAX_DOFS     150


// The data handler gets these two info types in addition to the data types

#define DISCONNECTED -1
#define MESSAGE       0


// These get ORed together for DataTypesWanted

#define TRC_DATA              1
#define GTR_DATA              2
#define HTR_DATA              4
#define ANALOG_DATA          64
#define HTR2_DATA           128
#define DOF_DATA            256
#define FORCE_DATA          512


// Queried information

#define HIERARCHY             8
#define MARKER_LIST          16
#define ANALOG_NAMES         32
#define DOF_NAMES           257
#define IS_LIVE             258
#define IS_RECORDING        259
#define CONTEXT_FRAME_RATE  260


#define XEMPTY 9999999.


typedef struct sTrcFrame
{
    int    iFrame;
    float  Markers[MAX_MARKERS][3];      /* X,Y,Z */

} sTrcFrame;


typedef struct sGtrFrame
{
    int    iFrame;
    float  Segments[MAX_SEGMENTS][7];    /* X,Y,Z,aX,aY,aZ,Length */

} sGtrFrame;


typedef struct sHtr2Frame  /* Same as sGtrFrame */
{
    int    iFrame;
    float  Segments[MAX_SEGMENTS][7];    /* X,Y,Z,aX,aY,aZ,Length */

} sHtr2Frame;


typedef struct sDofFrame  /* Mostly Angles */
{
    int    iFrame;
    int    nDOFs;
    double DOFs[MAX_DOFS];

} sDofFrame;


typedef struct sHtrFrame
{
    int    iFrame;
    float  RootPosition[3];              /* X,Y,Z */
    float  Segments[MAX_SEGMENTS][4];    /* aX,aY,aZ,Length */

} sHtrFrame;


typedef struct sAnalogFrame
{
    int    iFrame;
    int    nSamples;
    int    nChannels;
    short  wData[1];   /* This will actually be nChannels*nSamples words */

} sAnalogFrame;


typedef struct sForceFrame
{
    int    iFrame;
    int    nSamples;
    int    nPlates;
    float  fData[7];   /* This will actually be nPlates*nSamples*7 floats */

} sForceFrame;


typedef struct sHierarchy
{
    int    nSegments;
    char **szSegmentNames;
    int   *iParents;

} sHierarchy;


typedef struct sMarkerList
{
    int    nMarkers;
    char **szMarkerNames;

} sMarkerList;


typedef struct sAnalogNames
{
    int    nChannels;
    char **szChannelNames;

} sAnalogNames;


typedef struct sDofNames
{
    int    nDOFs;
    char **szNames;

} sDofNames;



#ifdef  __cplusplus
extern "C" {
#endif

DLL int  EVaRT_SdkVersion();

DLL int  EVaRT_Connect(char *EVaRT_hostname);
DLL void EVaRT_Disconnect();
DLL int  EVaRT_IsConnected();

DLL int  EVaRT_Request(char *str);

DLL int  EVaRT_RequestMarkerList();
DLL int  EVaRT_RequestAnalogNames();
DLL int  EVaRT_RequestDofNames();
DLL int  EVaRT_RequestHierarchy();
DLL int  EVaRT_RequestHierarchy2();
DLL int  EVaRT_RequestFrame();

DLL int  EVaRT_SetDataTypesWanted(int DataTypes);

DLL int  EVaRT_StartStreaming();
DLL int  EVaRT_StopStreaming();
DLL int  EVaRT_IsStreaming();

DLL int  EVaRT_UDP_StartStreaming();
DLL int  EVaRT_UDP_StopStreaming();

DLL int  EVaRT_RequestStartRecording();
DLL int  EVaRT_RequestStopRecording();

DLL int  EVaRT_UDP_ReadNextFrame();
DLL int  EVaRT_UDP_ReadLatestFrame();

DLL int  EVaRT_UDP_ReadFrame();    /* Old code: Use ReadLatestFrame */
DLL int  EVaRT_RequestFrame_UDP(); /* Old code: For testing only */

DLL void EVaRT_Initialize();
DLL void EVaRT_Exit();


/* The user gets the data via the user supplied Data Handler Function */

DLL void EVaRT_SetDataHandlerFunc(int (*MyFunc)(int DataType, void *Data));


/* Euler angle utility functions */

#define ZYX_ORDER 1
#define XYZ_ORDER 2
#define YXZ_ORDER 3
#define YZX_ORDER 4
#define ZXY_ORDER 5
#define XZY_ORDER 6

DLL void  EVaRT_ConstructRotationMatrix(
        double angles[3],
        int iRotationOrder,
        double matrix[3][3]);

DLL void  EVaRT_ExtractEulerAngles(
        double matrix[3][3],
        int    iRotationOrder,
        double angles[3]);



#ifdef  __cplusplus
}
#endif

#endif
