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
@import "../../View/InputWindow.j"
@import "../../View/NodeEditorView.j"
@import "../../Controller/NodeEditorController.j"


@implementation EncodingFlowEditorModuleController : CPViewController
{
  @outlet NodeEditorView  editorView;
  @outlet CPView          elementView;
  @outlet GraphListView   graphListView;
  @outlet CPButtonBar     graphButtonBar;

  id editorController;
  TextInput input;
}


#pragma mark -
#pragma mark Initialization

/*! called at cib awaking
*/
- (void)awakeFromCib
{
  [editorView setBackgroundColor:[CPColor whiteColor]];
  //graphListView=[[GraphListView alloc] initWithFrame:CPRectMake(0,260,300,270)];

  var buttons=[CPArray array];
  var minusButton             = [CPButtonBar minusButton];
  var plusButton             = [CPButtonBar plusButton];
  [plusButton setTitle:@"Load"];


  var loadButton=[[CPButton alloc] initWithFrame:CGRectMake(0, 0, 55, 25)];
  [loadButton setTitle:@"Load"];
  [loadButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];
  [loadButton setTarget:self];
  [loadButton setAction:@selector(load:)];

  var saveButton=[[CPButton alloc] initWithFrame:CGRectMake(0, 0, 55, 25)];
  [saveButton setTitle:@"Save"];
  [saveButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];
  [saveButton setTarget:self];
  [saveButton setAction:@selector(save:)];

  var newButton=[[CPButton alloc] initWithFrame:CGRectMake(0, 0, 55, 25)];
  [newButton setTitle:@"New"];
  [newButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];
  [newButton setTarget:self];
  [newButton setAction:@selector(new:)];

  [buttons addObject:loadButton];
  [buttons addObject:saveButton];
  [buttons addObject:newButton];

  [graphButtonBar setButtons:buttons];
  editorController=[[NodeEditorController alloc] initWithView:editorView];

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
- (void)save:(id)sender
{
  CPLog.debug("SAVING Encoding Graph:");
  [[CPNotificationCenter defaultCenter]
    postNotificationName:SaveNodeEditorView
    object:[graphListView refresh]
    userInfo:nil];
  
}

- (void)new:(id)sender
{
  CPLog.debug("New Encoding Graph:");
  /*asking for a name*/
  input=[[InputWindow alloc] initWithTitle:@"Name for the new Graph" andText:@"please enter a Name for the new Graph."];
  [input setDelegate:self];
  /*
  [[CPNotificationCenter defaultCenter]
    postNotificationName:NewNodeEditorView
    object:self
    userInfo:nil];*/
}
- (void)_new:(CPString)name
{
  CPLog.debug("New Encoding Graph execute:");
  
  [[CPNotificationCenter defaultCenter]
    postNotificationName:NewNodeEditorView
    object:name
    userInfo:nil];
}
  
- (void)load:(id)sender
{
  CPLog.debug("LOADING Encoding Graph:"+[graphListView selectedId]);
  if([graphListView selectedId]){
    [[CPNotificationCenter defaultCenter]
      postNotificationName:LoadNodeEditorView
      object:self
      userInfo:[graphListView selectedId]];
  }else{
    var alert=[CPAlert alertWithError:@"please select a graph from the list"];
    [alert runModal];
  }
}

- (void)alertDidEnd:(CPAlert)anAlert returnCode:(int)tag
{
  var name=[input inputText];
  if (tag === 1)
    [self _new:name];
  [CPTimer scheduledTimerWithTimeInterval:0.5 target:graphListView selector:@selector(refresh) userInfo:nil repeats:NO];

}

@end


// add this code to make the CPLocalizedString looking at
// the current bundle.
function CPBundleLocalizedString(key, comment)
{
    // DO NOT FORGET TO CHANGE THE CLASS NAME HERE
    return CPLocalizedStringFromTableInBundle(key, nil, [CPBundle bundleForClass:TNSampleTabModuleController], comment);
}
