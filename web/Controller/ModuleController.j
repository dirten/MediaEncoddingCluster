
/*
@import "../ModulesSource/ToolbarProfileModule/ProfileModuleController.j"
@import "../ModulesSource/ToolbarEncodingModule/EncodingModuleController.j"
@import "../ModulesSource/EncodingFlowEditorModule/EncodingFlowEditorModuleController.j"
*/
ModuleTypeToolbar = @"toolbar";
ModuleTypeTab = @"tab";


/*******************************************************************************
*
*
*******************************************************************************/
@implementation ModuleController: CPObject
/*******************************************************************************
*
*
*******************************************************************************/
{
  CPString                        _modulesPath                    @accessors(property=modulesPath);
  CPArray                         _bundles;
  TNToolbar                       _mainToolbar                    @accessors(property=mainToolbar);
  CPView                          _mainModuleView                 @accessors(property=mainView);
  CPDictionary                    _loadedToolbarModules           @accessors(getter=loadedToolbarModules);

  CPToolbarItem                   _currentToolbarItem;
  CPView                          _currentToolbarModule;
  id growl;
  CPDictionary moduleClass;

}


/*******************************************************************************
*
*
*******************************************************************************/
-(id)init{
  self = [super init];
  _loadedToolbarModules                   = [CPDictionary dictionary];
  growl=[TNGrowlCenter defaultCenter];
  moduleClass=[CPDictionary dictionary];
  /*
  [moduleClass setObject:[EncodingFlowEditorModuleController class] forKey:@"EncodingFlowEditorModuleController"];
  [moduleClass setObject:[EncodingModuleController class] forKey:@"EncodingModuleController"];
  [moduleClass setObject:[ProfileModuleController class] forKey:@"ProfileModuleController"];
  */
  return self;
}
/*******************************************************************************
*
*
*******************************************************************************/
-(void)load
{
    var request     = [CPURLRequest requestWithURL:[CPURL URLWithString:@"Modules/modules.plist"]],
        connection  = [CPURLConnection connectionWithRequest:request delegate:self];

    _moduleLoadingStarted = YES;
    [connection cancel];
    [connection start];

}
#pragma mark -
#pragma mark Delegates

/*******************************************************************************
*
*
*******************************************************************************/
- (void)connection:(CPURLConnection)connection didReceiveData:(CPString)data
{
    var cpdata = [CPData dataWithRawString:data];
    _modulesPList           = [cpdata plistObject];
    _numberOfModulesToLoad  = [[_modulesPList objectForKey:@"Modules"] count];
    for(a=0;a<_numberOfModulesToLoad;a++){
      var module  = [[_modulesPList objectForKey:@"Modules"] objectAtIndex:a],
          path    = _modulesPath + [module objectForKey:@"folder"],
          bundle  = [CPBundle bundleWithPath:path];
      CPLog.debug("Loading " + [CPBundle bundleWithPath:path]);
      [bundle loadWithDelegate:self];
    }
}

-(void)setSideView:(CPView)aView{
}


#pragma mark -
#pragma mark Modules loading

/*******************************************************************************
*
*
*******************************************************************************/
- (void)manageToolbarItemLoad:(CPBundle)aBundle
{
  
    var currentModuleController,
        defaults                = [CPUserDefaults standardUserDefaults],
        moduleName              = [aBundle objectForInfoDictionaryKey:@"CPBundleName"],
        moduleLabel             = [aBundle objectForInfoDictionaryKey:@"PluginDisplayName"],
        moduleTabIndex          = [aBundle objectForInfoDictionaryKey:@"TabIndex"],
        moduleToolTip           = [aBundle objectForInfoDictionaryKey:@"ToolTip"],
        supportedTypes          = [aBundle objectForInfoDictionaryKey:@"SupportedEntityTypes"],
        moduleToolbarIndex      = [aBundle objectForInfoDictionaryKey:@"ToolbarIndex"],
        toolbarOnly             = [aBundle objectForInfoDictionaryKey:@"ToolbarItemOnly"],
        mandatoryPermissions    = [aBundle objectForInfoDictionaryKey:@"MandatoryPermissions"],
        bundleLocale            = [aBundle objectForInfoDictionaryKey:@"CPBundleLocale"],
        frame                   = [_mainModuleView bounds],
        moduleToolbarItem       = [[CPToolbarItem alloc] initWithItemIdentifier:moduleName];

    if ([moduleLabel isKindOfClass:CPDictionary] && bundleLocale)
        moduleLabel = [moduleLabel objectForKey:[defaults objectForKey:@"CPBundleLocale"]] || [moduleLabel objectForKey:@"en"];

    if ([moduleToolTip isKindOfClass:CPDictionary] && bundleLocale)
        moduleToolTip = [moduleToolTip objectForKey:[defaults objectForKey:@"CPBundleLocale"]];
    CPLog.debug("IconPath:"+[aBundle resourcePath]);
    [moduleToolbarItem setLabel:moduleLabel];
    [moduleToolbarItem setImage:[[CPImage alloc] initWithContentsOfFile:[aBundle pathForResource:@"icon.png"] size:CPSizeMake(32, 32)]];
    [moduleToolbarItem setAlternateImage:[[CPImage alloc] initWithContentsOfFile:[aBundle pathForResource:@"icon-alt.png"] size:CPSizeMake(32, 32)]];
    [moduleToolbarItem setToolTip:moduleToolTip];

    // if toolbar item only, no cib

    if (toolbarOnly)
    { 
        var classstring=[aBundle objectForInfoDictionaryKey:@"CPPrincipalClass"];
        
        CPLog.debug("CPPrincipalClass-String:"+classstring);
        CPLog.debug("CPPrincipalClass-Class:"+CPClassFromString(classstring));
        CPLog.debug("CPPrincipalClass-Class:"+objj_getClass(classstring));

        CPLog.debug("[aBundle principalClass]:"+[aBundle principalClass]);
        currentModuleController =  [[[aBundle principalClass] alloc] init];
        CPLog.debug("currentModuleController  "+currentModuleController  );
        //[currentModuleController setToolbarItemOnly:YES];
        [moduleToolbarItem setTarget:currentModuleController];
        [moduleToolbarItem setAction:@selector(toolbarItemClicked:)];
    }
    else
    {
        currentModuleController  = [self _loadLocalizedModuleController:bundleLocale forBundle:aBundle];
        CPLog.debug("currentModuleController  "+currentModuleController  );
        //[currentModuleController setToolbarItemOnly:NO];
        [[currentModuleController view] setAutoresizingMask:CPViewWidthSizable | CPViewHeightSizable];
        //[[currentModuleController view] setBackgroundColor:_toolbarModuleBackgroundColor];
        [moduleToolbarItem setTarget:self];
        [moduleToolbarItem setAction:@selector(didToolbarModuleClicked:)];
    }

    [_mainToolbar addItem:moduleToolbarItem withIdentifier:moduleName];
    [_mainToolbar setPosition:moduleToolbarIndex forToolbarItemIdentifier:moduleName];

    /*[currentModuleController initializeModule];
    [currentModuleController setName:moduleName];
    [currentModuleController setToolbarItem:moduleToolbarItem];
    [currentModuleController setToolbar:_mainToolbar];
    [currentModuleController setLabel:moduleLabel];
    [currentModuleController setModuleType:ModuleTypeToolbar];
    [currentModuleController setMandatoryPermissions:mandatoryPermissions];
    //[currentModuleController setViewPermissionDenied:viewPermissionDenied];
    */
    [_loadedToolbarModules setObject:currentModuleController forKey:moduleName];

    //[currentModuleController _beforeWillLoad];

   [_mainToolbar reloadToolbarItems];

    CPLog.debug("Tabbtool loaded");
   [growl pushNotificationWithTitle:@"Module loaded" message:moduleName+" sucessfull loaded!"];
  [[CPRunLoop currentRunLoop] performSelectors]; 
  [[CPRunLoop mainRunLoop] performSelectors]; 


}

/*******************************************************************************
*
*
*******************************************************************************/
- (void)_loadLocalizedModuleController:(CPString)bundleLocale forBundle:(CPBundle)aBundle
{
    var defaults = [CPUserDefaults standardUserDefaults],
        moduleIdentifier = [aBundle objectForInfoDictionaryKey:@"CPBundleIdentifier"],
        moduleCibName = [aBundle objectForInfoDictionaryKey:@"CibName"],
        localizedCibName = @"en.lproj/" + moduleCibName,
        localizationStringsURL = [aBundle pathForResource:"en.lproj/Localizable.xstrings"],
        englishStringsURL = [aBundle pathForResource:@"en.lproj/Localizable.xstrings"];

        CPLog.debug("aBundle"+aBundle);
    // we don't use CPURLConnection because what is important is the error code
    // not the content that vary accross servers...
    var req = new XMLHttpRequest();
    req.open("GET", localizationStringsURL, false);
    req.send(null);
    if (req.status == 200)
    {
        CPLog.debug("MODULE LOADER: " + moduleIdentifier + " : Found the translation en" 
            + " strings information as expected at " + localizationStringsURL)
        //var plist = [CPPropertyListSerialization propertyListFromData:[CPData dataWithRawString:req.responseText] format:nil];

        //[aBundle setValue:plist forTable:@"Localizable"];
        //var classname=[aBundle objectForInfoDictionaryKey:@"CPPrincipalClass"];
        //CPLog.debug("PricipalClass+"+CPClassFromString([aBundle objectForInfoDictionaryKey:@"CPPrincipalClass"]));
        //CPLog.debug("CPPrincipalClass-Window:"+classname+"->"+[aBundle principalClass]);

        //CPLog.debug("localizedCibName:"+localizedCibName);
        //CPLog.debug("localizedCibName:"+localizationStringsURL);
        //var instance=[CPClassFromString([aBundle objectForInfoDictionaryKey:@"CPPrincipalClass"]) alloc];
        //CPLog.debug("Instance:"+instance);

        //return [[[aBundle objectForInfoDictionaryKey:@"CPPrincipalClass"] alloc] initWithCibName:localizedCibName bundle:aBundle];
        //return [instance initWithCibName:localizedCibName bundle:aBundle];
        
        if([aBundle principalClass]){
          return [[[aBundle principalClass] alloc] initWithCibName:localizedCibName bundle:aBundle];
        } else {
          return [[[elementClass objectForKey:[aBundle objectForInfoDictionaryKey:@"CPPrincipalClass"]] alloc] initWithCibName:localizedCibName bundle:aBundle];
        }
        //return [[aBundle principalClass] alloc];
    }
    else
    {
        CPLog.debug("MODULE LOADER: " + moduleIdentifier + " : Unable to  the translation " + [defaults objectForKey:@"CPBundleLocale"]
            + ". Getting english at URL " + englishStringsURL)

        var req = new XMLHttpRequest();
        req.open("GET", englishStringsURL, false);
        req.send(null);
        CPLog.debug("Response:"+req.responseText);
        var plist = [CPPropertyListSerialization propertyListFromData:[CPData dataWithRawString:req.responseText] format:nil],
            localizedCibName = @"en.lproj/" + moduleCibName;
        CPLog.debug("PList:"+plist);

        //[aBundle setDictionary:plist forTable:@"Localizable"];
        //var instance=[[aBundle principalClass] alloc];
        //return [instance initWithCibName:localizedCibName bundle:aBundle];
    }
}



#pragma mark -
#pragma mark Delegates

/*******************************************************************************
*
*
*******************************************************************************/
- (void)bundleDidFinishLoading:(CPBundle)aBundle
{
  CPLog.debug("bundleDidFinishLoading"+aBundle);
  var moduleInsertionType = [aBundle objectForInfoDictionaryKey:@"InsertionType"];
  [_bundles addObject:aBundle];

  if (moduleInsertionType == ModuleTypeTab)
      [self manageTabItemLoad:aBundle];
  else if (moduleInsertionType == ModuleTypeToolbar)
      [self manageToolbarItemLoad:aBundle];
}

#pragma mark -
#pragma mark Actions

/*******************************************************************************
*
*
*******************************************************************************/
- (IBAction)didToolbarModuleClicked:(id)sender
{
    var module  = [_loadedToolbarModules objectForKey:[sender itemIdentifier]],
        oldModule;
    if (_currentToolbarModule)
    {
        /*
        var moduleBundle    = [_currentToolbarModule bundle],
            iconPath        = [moduleBundle pathForResource:[moduleBundle objectForInfoDictionaryKey:@"ToolbarIcon"]];
            */
        oldModule = _currentToolbarModule;
        //[_currentToolbarItem setImage:[[CPImage alloc] initWithContentsOfFile:iconPath size:CPSizeMake(32,32)]];

        //[_currentToolbarModule willHide];
        [[_currentToolbarModule view] removeFromSuperview];
        _currentToolbarModule   = nil;
        _currentToolbarItem     = nil;

        [_mainToolbar deselectToolbarItem];
    }

    if (module != oldModule)
    {
        CPLog.debug("click module 123"+module);
        /*
        var bounds          = [_mainModuleView bounds],
            moduleBundle    = [module bundle],
            iconPath        = [moduleBundle pathForResource:[moduleBundle objectForInfoDictionaryKey:@"AlternativeToolbarIcon"]];

        [sender setImage:[[CPImage alloc] initWithContentsOfFile:iconPath size:CPSizeMake(32,32)]];
        */
        [[module view] setFrame:[_mainModuleView bounds]];
        //[module willShow];

        [_mainModuleView addSubview:[module view]];

        _currentToolbarModule   = module;
        _currentToolbarItem     = sender;

        [_mainToolbar selectToolbarItem:sender];
    }
}
@end
