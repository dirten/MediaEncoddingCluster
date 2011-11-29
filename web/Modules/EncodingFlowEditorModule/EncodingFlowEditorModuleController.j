/*
 * TNSampleTabModule.j
 *
 * Copyright (C) 2010 Antoine Mercadal <antoine.mercadal@inframonde.eu>
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


@import <Foundation/Foundation.j>
@import "../../View/NodeEditorView.j"


@implementation EncodingFlowEditorModuleController : CPViewController
{
  @outlet NodeEditorView  editorView;
  @outlet CPView          elementView;
  @outlet CPView          graphListView;
}


#pragma mark -
#pragma mark Initialization

/*! called at cib awaking
*/
- (void)awakeFromCib
{
  [editorView setBackgroundColor:[CPColor whiteColor]];
  graphListView=[[GraphListView alloc] initWithFrame:CPRectMake(0,260,300,270)];
}

#pragma mark -
#pragma mark TNModule overrides

-(void)toolbarItemClicked:(id)aSender
{

}
/*! called when module is loaded
*/
- (void)willLoad
{
    [super willLoad];

    var center = [CPNotificationCenter defaultCenter];
    [center addObserver:self selector:@selector(_didUpdateNickName:) name:TNStropheContactNicknameUpdatedNotification object:_entity];
}

/*! called when module is unloaded
*/
- (void)willUnload
{
    [super willUnload];
}

/*! called when module becomes visible
*/
- (BOOL)willShow
{
    if (![super willShow])
        return NO;

    [fieldName setStringValue:[_entity nickname]];
    [fieldJID setStringValue:[_entity JID]];

    return YES;
}

/*! called when module becomes unvisible
*/
- (void)willHide
{
    // you should close all your opened windows and popover now.

    [super willHide];
}

/*! called when user permissions changed
*/
- (void)permissionsChanged
{
    [super permissionsChanged];

    // You may need to update your GUI to disable some
    // controls if permissions changed
}


#pragma mark -
#pragma mark Notification handlers


#pragma mark -
#pragma mark Utilities

// put your utilities here


#pragma mark -
#pragma mark Actions


#pragma mark -
#pragma mark Delegates

// put your delegates here

@end


// add this code to make the CPLocalizedString looking at
// the current bundle.
function CPBundleLocalizedString(key, comment)
{
    // DO NOT FORGET TO CHANGE THE CLASS NAME HERE
    return CPLocalizedStringFromTableInBundle(key, nil, [CPBundle bundleForClass:TNSampleTabModuleController], comment);
}
