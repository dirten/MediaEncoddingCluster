/*
* AppController.j
* NewApplication
*
* Created by You on February 22, 2011.
* Copyright 2011, Your Company All rights reserved.
*/

@import <Foundation/CPObject.j>
@import <AppKit/AppKit.j>
@import "ProfileList.j"
@import "Navigator.j"
@import "ProfileEditView.j"

var SliderToolbarItemIdentifier = "SliderToolbarItemIdentifier",
AddToolbarItemIdentifier = "AddToolbarItemIdentifier",
RemoveToolbarItemIdentifier = "RemoveToolbarItemIdentifier";


@implementation AppController : CPObject
    {
        var jsonData;
    }

    - (void)applicationDidFinishLaunching:(CPNotification)aNotification
    {
        var theWindow = [[CPWindow alloc] initWithContentRect:CGRectMakeZero() styleMask:CPBorderlessBridgeWindowMask],
        contentView = [theWindow contentView],
        toolBar = [[CPToolbar alloc] initWithIdentifier:"Toolbar"],
        bounds = [contentView bounds];

        [toolBar setDelegate:self];
        [toolBar setVisible:true];
        [theWindow setToolbar:toolBar];


        [contentView setBackgroundColor:[CPColor blackColor]];
        //var profilePanel =[[ProfileEditView alloc] initWithContentRect:CGRectMake(15,150,225,125) styleMask:CPHUDBackgroundWindowMask|CPClosableWindowMask];
        //var profilePanel =[[ProfileEditView alloc] init];
        //[profilePanel setFloatingPanel:YES];
        //[profilePanel orderFront:self];
        //[profilePanel setTitle:"Inspector"];
        /*
        var HUDPanel =[[CPPanel alloc] initWithContentRect:CGRectMake(0,0,225,125) styleMask:CPHUDBackgroundWindowMask|CPClosableWindowMask];
        [HUDPanel setFloatingPanel:YES];
        [HUDPanel orderFront:self];
        [HUDPanel setTitle:"Inspector"];

        var panelContentView = [HUDPanel contentView],
        centerX = (CGRectGetWidth([panelContentView bounds]) - 135.0) / 2.0;
        var scaleSlider = [[CPSlider alloc] initWithFrame:CGRectMake(centerX, 13.0, 135.0, 24.0)];

        [scaleSlider setMinValue:50];
        [scaleSlider setMaxValue:150];
        [scaleSlider setValue:100];
        [panelContentView addSubview:scaleSlider];
        var scaleStartLabel = [self labelWithTitle:"50%"],
        scaleEndLabel = [self labelWithTitle:"150%"];

        [scaleStartLabel setFrameOrigin:CGPointMake( centerX - CGRectGetWidth([scaleStartLabel frame]), 10)];
        [scaleEndLabel setFrameOrigin: CGPointMake(CGRectGetMaxX([scaleSlider frame]), 10)];

        [panelContentView addSubview:scaleStartLabel];
        [panelContentView addSubview:scaleEndLabel];

        */

        var listScrollView = [[CPScrollView alloc] initWithFrame:CGRectMake(0,0,200,CGRectGetHeight(bounds)-58)];
        [listScrollView setAutohidesScrollers:YES];
        [listScrollView setAutoresizingMask:CPViewHeightSizable];
        [[listScrollView contentView] setBackgroundColor:[CPColor colorWithRed:221.0/255.0 green:228.0/255.0 blue:235.0/255.0 alpha:1.0]];


        var photosListItem = [[CPCollectionViewItem alloc] init];
        [photosListItem setView:[[PhotosListCell alloc] initWithFrame:CGRectMakeZero()]];

        listCollectionView = [[CPCollectionView alloc] initWithFrame:CGRectMake(0, 0, 200, 0)];

        [listCollectionView setDelegate:self]; //we want delegate methods
        [listCollectionView setItemPrototype:photosListItem]; //set the item prototype

        [listCollectionView setMinItemSize:CGSizeMake(20.0, 45.0)];
        [listCollectionView setMaxItemSize:CGSizeMake(1000.0, 45.0)];
        [listCollectionView setMaxNumberOfColumns:1]; //setting a single column will make this appear as a vertical list

        [listCollectionView setVerticalMargin:0.0];
        [listCollectionView setAutoresizingMask:CPViewWidthSizable];

        //finally, we put our collection view inside the scroll view as it's document view, so it can be scrolled
        //[listScrollView setDocumentView:listCollectionView];

        [contentView addSubview:listScrollView];



        leftsets = [CPDictionary dictionary];
        [leftsets setObject:"nil" forKey:"test"];
        CPLog.debug(leftsets);
        [listCollectionView setContent:[[leftsets allKeys] copy]];

        var textColumn = [[CPTableColumn alloc] initWithIdentifier:@"TextColumn"];
        [textColumn setWidth:190.0];
        [textColumn setEditable:YES];

        var textColumn2 = [[CPTableColumn alloc] initWithIdentifier:@"TextColumn2"];
        [textColumn2 setWidth:190.0];
        [textColumn2 setEditable:YES];
        navi=[[[MyNavigator alloc] initWithFrame:[[listScrollView contentView] bounds]] init];
        //[navi setBackgroundColor:[CPColor colorWithRed:221.0/255.0 green:228.0/255.0 blue:235.0/255.0 alpha:1.0]];
        
        //[navi setDelegate:self];
        //CPLog.debug(navigator);
        //[navi addTableColumn:textColumn2];
        //[navi setOutlineTableColumn:textColumn2];
        //[navi setDataSource:navi];

        //treeView=[[CPOutlineView alloc] initWithFrame:[[listScrollView contentView] bounds]];
        [listScrollView setDocumentView:navi];

        //[treeView addTableColumn:textColumn];
        //[treeView setOutlineTableColumn:textColumn];

        //subitems=[CPDictionary dictionaryWithObjects:[[@"glossary 1"], [@"proj 1", @"proj 2", @"proj 3"]] forKeys:[@"Glossaries", @"Projects"]];
        //items = [CPDictionary dictionaryWithObjects:[[@"glossary 1"], [@"proj 1", @"proj 2", @"proj 3"]] forKeys:[@"Glossaries", @"Projects"]];

        //CPLog.debug(items);
        //[treeView setDataSource:self];
        //[treeView setDelegate:self];


        
        tableView=[[CPTableView alloc] initWithFrame:CGRectMake(0, 0, CGRectGetWidth(bounds) - 400, 300)];
        var idcolumn = [[CPTableColumn alloc] initWithIdentifier:[CPString stringWithFormat:@"%d", 1]];
        [[idcolumn headerView] setStringValue:"Id"];
        [[idcolumn headerView] sizeToFit];
        [idcolumn setWidth:280];
        [tableView addTableColumn:idcolumn];
        var namecolumn = [[CPTableColumn alloc] initWithIdentifier:[CPString stringWithFormat:@"%d", 2]];
        [[namecolumn headerView] setStringValue:"Profile Name"];
        [[namecolumn headerView] sizeToFit];
        [namecolumn setWidth:400];
        [tableView addTableColumn:namecolumn];
        /*
        for (var i = 1; i <= 2; i++)
            {
                var column = [[CPTableColumn alloc] initWithIdentifier:[CPString stringWithFormat:@"%d", i]];

                // Modify the column header
                [[column headerView] setStringValue:"Number"];
                [[column headerView] sizeToFit];
                // The column width is the same as the column header width
                //[column setWidth:[[column headerView] frame].size.width];

                // PLL : we try this to show text is aligned to the left
                [column setWidth:280];

                var dataView = [column dataView];
                // [dataView setAlignment:CPCenterTextAlignment];
                // [dataView setVerticalAlignment:CPCenterVerticalTextAlignment];

                ////[dataView setValue:CPCenterTextAlignment forThemedAttributeName:"alignment"];
                ////[dataView setValue:CPCenterVerticalTextAlignment forThemedAttributeName:"vertical-alignment"];
                ////[dataView setValue:CPLineBreakByWordWrapping forThemedAttributeName:"line-break-mode"];


                [tableView addTableColumn:column];
            }*/
            [tableView setDataSource:self];
            [tableView setDelegate:self];
            [tableView setDoubleAction:@selector(doubleClicked)];
            
            var scrollView = [[CPScrollView alloc] initWithFrame:CGRectMake(200, 0, CGRectGetWidth(bounds)-400, CGRectGetHeight(bounds)-58)];
            [scrollView setAutoresizingMask:CPViewWidthSizable | CPViewHeightSizable];
            [scrollView setDocumentView:tableView];

            [contentView addSubview:scrollView];
            
            // Uncomment the following line to turn on the standard menu bar.
        //[CPMenu setMenuBarVisible:YES];
        var request = [CPURLRequest requestWithURL:"http://localhost:8080/api/v1/profile"];
        [request setHTTPMethod:"GET"];
        CPLog.debug(request.HTTPMethod);
        // see important note about CPJSONPConnection above
        var connection = [CPURLConnection connectionWithRequest:request delegate:self];

        [theWindow orderFront:self];
    }
- (void)connection:(CPURLConnection)aConnection didReceiveData:(CPString)data
{
    //this method is called when the network request returns. the data is the returned
    //information from flickr. we set the array of photo urls as the data to our collection view

    //[self addImageList:data.photos.photo withIdentifier:lastIdentifier];
    //CPLog.debug("Data"+data);
    jsonData=[data objectFromJSON];
    //CPLog.debug("json="+jsonData.requestId);
    [tableView reloadData];
}
- (void)connection:(CPURLConnection)aConnection didReceiveResponse:(CPHTTPURLResponse)response
{
    //this method is called when the network request returns. the data is the returned
    //information from flickr. we set the array of photo urls as the data to our collection view

    //[self addImageList:data.photos.photo withIdentifier:lastIdentifier];
    CPLog.debug(response);
}

- (void)connection:(CPURLConnection)aConnection didFailWithError:(CPString)error
{
    alert(error); //a network error occurred
}

    - (CPTextField)labelWithTitle:(CPString)aTitle {
        var label = [[CPTextField alloc] initWithFrame:CGRectMakeZero()];
        [label setStringValue:aTitle];
        [label setTextColor:[CPColor whiteColor]];
        [label sizeToFit];
        return label;
    }
    //these two methods are the toolbar delegate methods, and tell the toolbar what it should display to the user

    - (CPArray)toolbarAllowedItemIdentifiers:(CPToolbar)aToolbar
    {
        return [self toolbarDefaultItemIdentifiers:aToolbar];
    }

    - (CPArray)toolbarDefaultItemIdentifiers:(CPToolbar)aToolbar
    {
        return [AddToolbarItemIdentifier, RemoveToolbarItemIdentifier, CPToolbarFlexibleSpaceItemIdentifier, SliderToolbarItemIdentifier];
    }

    //this delegate method returns the actual toolbar item for the given identifier

- (CPToolbarItem)toolbar:(CPToolbar)aToolbar itemForItemIdentifier:(CPString)anItemIdentifier willBeInsertedIntoToolbar:(BOOL)aFlag
{
    CPLog.debug("call toolbar");
    var toolbarItem = [[CPToolbarItem alloc] initWithItemIdentifier:anItemIdentifier];

    /*if (anItemIdentifier == SliderToolbarItemIdentifier)
    {
    [toolbarItem setView:[[PhotoResizeView alloc] initWithFrame:CGRectMake(0, 0, 180, 32)]];
    [toolbarItem setMinSize:CGSizeMake(180, 32)];
    [toolbarItem setMaxSize:CGSizeMake(180, 32)];
    [toolbarItem setLabel:"Scale"];
    }
    else */
        if (anItemIdentifier == AddToolbarItemIdentifier)
            {
                var image = [[CPImage alloc] initWithContentsOfFile:[[CPBundle mainBundle] pathForResource:"add.png"] size:CPSizeMake(30, 25)],
                highlighted = [[CPImage alloc] initWithContentsOfFile:[[CPBundle mainBundle] pathForResource:"addHighlighted.png"] size:CPSizeMake(30, 25)];

                [toolbarItem setImage:image];
                [toolbarItem setAlternateImage:highlighted];

                [toolbarItem setTarget:self];
                [toolbarItem setAction:@selector(add:)];
                [toolbarItem setLabel:"Add Photo List"];

                [toolbarItem setMinSize:CGSizeMake(32, 32)];
                [toolbarItem setMaxSize:CGSizeMake(32, 32)];
            }
            else if (anItemIdentifier == RemoveToolbarItemIdentifier)
                {
                    var image = [[CPImage alloc] initWithContentsOfFile:[[CPBundle mainBundle] pathForResource:"remove.png"] size:CPSizeMake(30, 25)],
                    highlighted = [[CPImage alloc] initWithContentsOfFile:[[CPBundle mainBundle] pathForResource:"removeHighlighted.png"] size:CPSizeMake(30, 25)];

                    [toolbarItem setImage:image];
                    [toolbarItem setAlternateImage:highlighted];

                    [toolbarItem setTarget:self];
                    [toolbarItem setAction:@selector(remove:)];
                    [toolbarItem setLabel:"Remove Photo List"];

                    [toolbarItem setMinSize:CGSizeMake(32, 32)];
                    [toolbarItem setMaxSize:CGSizeMake(32, 32)];
                }

                return toolbarItem;
            }
- (void)add:(id)sender
{
    alert([treeView itemAtRow:[treeView selectedRow]]);
    CPLog.debug([treeView dataSource]);
    //var string = prompt("Enter a tag to search Flickr for photos.");
    
}

- (void)remove:(id)sender
{
    //remove this photo
    //[self removeImageListWithIdentifier:[[photosets allKeys] objectAtIndex:[[listCollectionView selectionIndexes] firstIndex]]];
}
- (BOOL)selectionShouldChangeInTableView:(CPTableView)aTableView{
    return YES;
}
- (BOOL)tableView:(CPTableView)aTableView shouldSelectRow:(int)rowIndex{
    //CPLog.debug(rowIndex);
    return YES;
}
- (void) doubleClicked{
    CPLog.debug("double clicked");
    var profilePanel =[[ProfileEditView alloc] init];

}
- (void)tableViewSelectionDidChange:(CPNotification)aNotification{
    CPLog.debug("hello:"+[aNotification object]);
}

- (id)outlineView:(CPOutlineView)outlineView child:(int)index ofItem:(id)item
{
    CPLog("outlineView:%@ child:%@ ofItem:%@", outlineView, index, item);

    if (item === nil)
    {
        var keys = [items allKeys];
        console.log([keys objectAtIndex:index]);
        return [keys objectAtIndex:index];
    }
    else
    {
        var values = [items objectForKey:item];
        console.log([values objectAtIndex:index]);
        return [values objectAtIndex:index];
    }
}

- (BOOL)outlineView:(CPOutlineView)outlineView isItemExpandable:(id)item
{
    CPLog("outlineView:%@ isItemExpandable:%@", outlineView, item);

    var values = [items objectForKey:item];
    console.log(([values count] > 0));
    return ([values count] > 0);
}

- (int)outlineView:(CPOutlineView)outlineView numberOfChildrenOfItem:(id)item
{
    CPLog("outlineView:%@ numberOfChildrenOfItem:%@", outlineView, item);

    if (item === nil)
    {
        console.log([items count]);
        return [items count];
    }
    else
    {
        var values = [items objectForKey:item];
        console.log([values count]);
        return [values count];
    }
}

- (id)outlineView:(CPOutlineView)outlineView objectValueForTableColumn:(CPTableColumn)tableColumn byItem:(id)item
{
    CPLog("outlineView:%@ objectValueForTableColumn:%@ byItem:%@", outlineView, tableColumn, item);

    console.log(item);

    return item;
}


            @end
@implementation PhotosListCell : CPView
{
    CPTextField     label;
    CPView          highlightView;
}

- (void)setRepresentedObject:(JSObject)anObject
{
    if(!label)
    {
        label = [[CPTextField alloc] initWithFrame:CGRectInset([self bounds], 4, 4)];

        [label setFont:[CPFont systemFontOfSize:16.0]];
        [label setTextShadowColor:[CPColor whiteColor]];
        [label setTextShadowOffset:CGSizeMake(0, 1)];

        [self addSubview:label];
    }

    [label setStringValue:anObject];
    [label sizeToFit];

    [label setFrameOrigin:CGPointMake(10,CGRectGetHeight([label bounds]) / 2.0)];
}

- (void)setSelected:(BOOL)flag
{
    if(!highlightView)
    {
        highlightView = [[CPView alloc] initWithFrame:CGRectCreateCopy([self bounds])];
        [highlightView setBackgroundColor:[CPColor blueColor]];
    }

    if(flag)
    {
        [self addSubview:highlightView positioned:CPWindowBelow relativeTo:label];
        [label setTextColor:[CPColor whiteColor]];
        [label setTextShadowColor:[CPColor blackColor]];
    }
    else
    {
        [highlightView removeFromSuperview];
        [label setTextColor:[CPColor blackColor]];
        [label setTextShadowColor:[CPColor whiteColor]];
    }
}

@end

@implementation AppController (TableViewDataSource)

- (int)numberOfRowsInTableView:(CPTableView)tableView
{
    if(jsonData)
        return jsonData.data.length;
    return 0;
}

- (id)tableView:(CPTableView)tableView objectValueForTableColumn:(CPTableColumn)tableColumn row:(int)row
{
    if ([tableColumn identifier] == "icons")
	{
		// return a CPImage instance that will be displayed in the custom column (CPImageView)
        if (row % 2)
        	return _iconImage1;
        else if (row % 3)
        	return _iconImage3;
        else
        	return _iconImage2;
	}
    else
	{
		// return a CPString instance that will be displayed in a regular column (CPTextField)
            //CPLog.debug([tableColumn identifier]);
            if([tableColumn identifier]==1){
                return [CPString stringWithFormat:@"%s", jsonData.data[row].id ];
            }else
            if([tableColumn identifier]==2){
                return [CPString stringWithFormat:@"%s", jsonData.data[row].name ];
            }
	}
}

@end