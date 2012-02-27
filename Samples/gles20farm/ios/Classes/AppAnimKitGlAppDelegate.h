//
//  AppAnimKitGlAppDelegate.h
//  AppAnimKitGl
//
//  Created by astojilj on 12/22/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class AppAnimKitGlViewController;

@interface AppAnimKitGlAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    AppAnimKitGlViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet AppAnimKitGlViewController *viewController;

@end

