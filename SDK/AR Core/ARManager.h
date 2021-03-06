//
//  ARManager.h
//  PAR Works iOS SDK
//
//  Copyright 2012 PAR Works, Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//


#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import "ASIHTTPRequest.h"
#import "ASIFormDataRequest.h"
#import "ARSite.h"

@interface ARManager : NSObject <CLLocationManagerDelegate>
{
    NSString * _apiSecret;
    NSString * _apiKey;
    NSString * _appVersion;
    NSString * _appUsername;
    
    CLLocationManager * _locationManager;
    BOOL _locationEnabled;
}

+ (ARManager *)shared;
+ (id)allocWithZone:(NSZone *)zone;
- (id)copyWithZone:(NSZone *)zone;
- (id)init;


// ================================
// @name Authentication
// ================================

- (void)setApiKey:(NSString*)key andSecret:(NSString*)secret;

- (BOOL)locationEnabled;

- (void)setLocationEnabled:(BOOL)enabled;

// ================================
// @name Device Position and Pose
// ================================

/**
 @return The current location of the device.
 */
- (CLLocation*)deviceLocation;

/**
@return The current heading of the device.
*/
- (CLHeading*)deviceHeading;

// ========================
// @name Creating Requests
// ========================

/** Generates an URL for connecting to the AR API with the given properties.

  @param basePath The absolute path of the request on the server, such as @"/ar/site"
  
  @param args A dictionary of arguments that will be added to the url as GET params
*/
- (NSURL*)urlForRequest:(NSString*)basePath withPathArgs:(NSDictionary*)args;

/** Generates a new ASIHTTPRequest object for connecting to the AR API with the
  provided properties. Connections to the AR system should always be created with 
  this function, which adds authentication headers, metadata, and more to the 
  request. 

  @param path The absolute path you are requesting, such as @"/ar/site"
  
  @param method The request method, such as @"POST" or @"DELETE"
  
  @param args A set of arguments. If you are making a GET request, these arguments are
  passed as url parameters. For other requests, they are added to the POST body.
  
  @return A new ASIHTTPRequest or ASIFormDataRequest object. You should attach callbacks
  and/or a delegate to this request and then call startAsynchronous to start it.
*/
- (ASIHTTPRequest*)createRequest:(NSString*)path withMethod:(NSString*)method withArguments:(NSDictionary*)args;

// ========================
// @name Augmenting Photos
// ========================

/** Convenience function that takes the provided photo and creates a new ARAugmentedPhoto object.
  It automatically begins the augmentation process. To be notified when augmentation results have
  been received, listen for a NOTIF_AUGMENTED_PHOTO_UPDATED notification with the ARAugmentedPhoto 
  object as the notification object.

  @note If your application does not use location services, or if you want to augment an image using
  a specific ARSite of base images, use the functions in the ARSite to augment photos instead. 
  
  @warning You must call setLocationEnabled: YES before calling this method.
  
  @param image The image to be augmented. Must be > 2000 x 2000px to work well.
 
  @param completion A block method that will be executed when augmentation
  of the image has completed. The block has one parameter which is the
  augmented photo. The photo has properties related to its status at completion.

  @result An ARAugmentedPhoto object that represents the augmented photo. This object provides access to
  augmentation status and the overlays found within the image.
*/
- (ARAugmentedPhoto *)augmentPhotoUsingNearbySites:(UIImage*)image completion:(ARProcessingCompletionBlock)completion;

// ========================
// @name Managing and Finding Sites
// ========================

/** Adds a site to your account on the server with the given identifier.
 
  @param identifier The identifier of the site you'd like to create.
  
  @param completionBlock A block to call when the server response has been received.
  
*/
- (void)addSite:(NSString*)identifier withCompletionBlock:(void (^)(void))completionBlock;


/** Removes a site to your account on the server with the given identifier.
 
 @param identifier The identifier of the site you'd like to remove.
 
 @param completionBlock A block to call when the server response has been received.
 
 */
- (void)removeSite:(NSString*)identifier withCompletionBlock:(void (^)(void))completionBlock;


/** Retrieves a json list of all of the current user's sites based on the API key provided to
 the ARManager.
 
 @param completionBlock This block is called when the resonse has been received
 */
- (void)sitesForCurrentAPIKey:(void (^)(NSArray *sites))completionBlock;


/** Finds nearby sites using a given resolution.
 
 @param resolution We use a geohashing algorithm that produces a single hash 
 string from a lat/lon pair. The geohash algorithm subdivides the world into
 bounding boxes of a predefined resolution (e.g. size). The higher the
 resolution, the smaller the bounding box and the longer its geohash.
 If a geohash is a prefix of another one, it means that its bounding
 box contains the other geohash. The resolution parameter is basically
 a specification of how large a bounding box you want to search within.
 
 @param completionBlock A block to call when the server response has been received.
 The NSArray parameter contains the sites.
 
 */
- (void)findNearbySites:(int)resolution withCompletionBlock:(void (^)(NSArray *))completionBlock;

#pragma mark Convenience Functions for Image Picking

- (UIImage*)rotateImage:(UIImage*)img byOrientationFlag:(UIImageOrientation)orient;


// ========================
// @name Handling Errors
// ========================

- (BOOL)handleResponseErrors:(ASIHTTPRequest*)req;

/** A global convenience function for displaying error messages when improtant 
  requests fail. Based on the status code of the response, this function displays
  different UIAlertViews and notifies the user of the connection issue.

  @param req The failed request.
*/
- (void)criticalRequestFailed:(ASIHTTPRequest*)req;

#pragma mark -
#pragma mark Convenience Functions for Networking

- (NSString*)shaHashFor:(NSString*)data withKey:(NSString*)key;

@end
