@implementation MyNavigator :CPOutlineView
{
    var items;
}

-(id)init
{
    CPLog.debug("Nvaigator init");
    //items = [CPDictionary dictionaryWithObjects:[[@"glossary 2"], [@"proj 1", @"proj 2", @"proj 3"]] forKeys:[@"Glossaries", @"Projects"]];
    //items = [CPDictionary dictionaryWithObjects:[[@"Configuration",@"Nodes"],[], []] forKeys:[@"System",@"Profiles", @"Encodings"]];
    items = [CPDictionary dictionaryWithObjects:[[],[], []] forKeys:[@"Profiles", @"Encodings"]];
    var textColumn = [[CPTableColumn alloc] initWithIdentifier:@"TextColumn2"];
    [textColumn setWidth:190.0];
    [textColumn setEditable:YES];
    //[[textColumn headerView] setStringValue:"Main Menu"];
    [self addTableColumn:textColumn];
    [self setOutlineTableColumn:textColumn];
    [self setBackgroundColor:[CPColor colorWithRed:221.0/255.0 green:228.0/255.0 blue:235.0/255.0 alpha:1.0]];
    //[self setBackgroundColor:[CPColor colorWithRed:46.0/255.0 green:46.0/255.0 blue:50.0/255.0 alpha:1.0]];
    [self setAllowsEmptySelection:NO];
    [self setDataSource:self];
    [self setDelegate:self];
    [self selectRowIndexes:[CPIndexSet indexSetWithIndex:0] byExtendingSelection:NO];
  [[CPNotificationCenter defaultCenter]
    postNotificationName:CPOutlineViewSelectionDidChangeNotification
    object:self
    userInfo:nil];
    
  //[self tableViewSelectionDidChange:nil];
    //[self setTarget:self];
    return self;
}

- (id)outlineView:(CPOutlineView)outlineView child:(int)index ofItem:(id)item
{
    CPLog.debug("outlineView:%@ child:%@ ofItem:%@", outlineView, index, item);

    if (item === nil)
    {
        var keys = [items allKeys];
        //console.log([keys objectAtIndex:index]);
        return [keys objectAtIndex:index];
    }
    else
    {
        var values = [items objectForKey:item];
        //console.log([values objectAtIndex:index]);
        return [values objectAtIndex:index];
    }
}

- (BOOL)outlineView:(CPOutlineView)outlineView isItemExpandable:(id)item
{
    CPLog.debug("outlineView:%@ isItemExpandable:%@", outlineView, item);

    var values = [items objectForKey:item];
    //console.log(([values count] > 0));
    return ([values count] > 0);
}

- (int)outlineView:(CPOutlineView)outlineView numberOfChildrenOfItem:(id)item
{
    CPLog.debug("outlineView:%@ numberOfChildrenOfItem:%@", outlineView, item);

    if (item === nil)
    {
        //console.log([items count]);
        return [items count];
    }
    else
    {
        var values = [items objectForKey:item];
        //console.log([values count]);
        return [values count];
    }
}

- (id)outlineView:(CPOutlineView)outlineView objectValueForTableColumn:(CPTableColumn)tableColumn byItem:(id)item
{
    CPLog.debug("outlineView:%@ objectValueForTableColumn:%@ byItem:%@", outlineView, tableColumn, item);

    //console.log(item);

    return item;
}
- (void)tableViewSelectionDidChange:(CPNotification)aNotification
{
    CPLog.debug("selection");
}
- (BOOL)selectionShouldChangeInTableView:(CPOutlineView)outlineView
{
    CPLog("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    return YES;
}
-(BOOL)outlineView:(CPOutlineView)aTableView shouldSelectItem:(int)rowIndex
{
    //CPLog("bla"+rowIndex);
    return YES;
}
- (BOOL)outlineView:(CPOutlineView)aTableView shouldSelectTableColumn:(CPTableColumn)aTableColumn
{
    CPLog("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    return YES;
}
- (void)outlineView:(CPOutlineView)outlineView didClickTableColumn:(CPTableColumn)tableColumn
{
    alert("bla");
}
@end
