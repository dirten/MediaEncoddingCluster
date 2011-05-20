
// Initialize MochaUI when the DOM is ready
window.addEvent('domready', function(){
  MochaUI.Desktop = new MochaUI.Desktop();
  MochaUI.Dock = new MochaUI.Dock();

new MochaUI.Column({
    id: 'mainColumn',
    placement: 'main',
    width:900
  });

  new MochaUI.Panel({
    id: 'mainPanel',
    title: 'System Setup',
    loadMethod: 'xhr',
    contentURL: 'pages/setup.html',
    column: 'mainColumn',
    height: 200
  });

	
  MochaUI.Desktop.desktop.setStyles({
    'background': '#fff',
    'visibility': 'visible'
  });

});

// This runs when a person leaves your page.
window.addEvent('unload', function(){
  if (MochaUI) MochaUI.garbageCleanUp();
});