/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client;

import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.core.client.GWT;
import com.google.gwt.core.client.JavaScriptObject;
import com.google.gwt.core.client.JsArray;
import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.http.client.Request;
import com.google.gwt.http.client.RequestBuilder;
import com.google.gwt.http.client.RequestCallback;
import com.google.gwt.http.client.RequestException;
import com.google.gwt.http.client.Response;
import com.google.gwt.uibinder.client.UiBinder;
import com.google.gwt.user.client.Window;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.DockLayoutPanel;
import com.google.gwt.user.client.ui.HTML;
import com.google.gwt.user.client.ui.HTMLPanel;
import com.google.gwt.user.client.ui.RootLayoutPanel;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.VerticalPanel;
import com.sencha.gxt.core.client.GXT;
import com.sencha.gxt.core.client.util.Margins;
import com.sencha.gxt.state.client.CookieProvider;
import com.sencha.gxt.state.client.StateManager;
import com.sencha.gxt.widget.core.client.ContentPanel;
import com.sencha.gxt.widget.core.client.Dialog;
import com.sencha.gxt.widget.core.client.container.BorderLayoutContainer;
import com.sencha.gxt.widget.core.client.container.BorderLayoutContainer.BorderLayoutData;
import com.sencha.gxt.widget.core.client.container.MarginData;
import com.sencha.gxt.widget.core.client.container.SimpleContainer;
import com.sencha.gxt.widget.core.client.container.Viewport;
import de.mec.client.ProfileList;
import de.mec.client.uitest.MainPage;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author jan.hoelscher
 */
public class MainUI implements EntryPoint {

    /*
    interface MyUiBinder extends UiBinder<DockLayoutPanel, MainUI> {
    }
    private static MyUiBinder uiBinder = GWT.create(MyUiBinder.class);

    private void displayError(final String error) {
        GWT.log(error);
    }
*/
    public void onModuleLoad() {
        
        /*
        GWT.setUncaughtExceptionHandler(new GWT.UncaughtExceptionHandler() {
            public void onUncaughtException(Throwable e) {
                final Dialog dialogBox = new Dialog();
                dialogBox.setHeadingText("Unhandled Error");
                //dialogBox.setAnimationEnabled(true);
                //final Button closeButton = new Button("Close");
                // We can set the id of a widget by accessing its Element
                //closeButton.getElement().setId("closeButton");
                //final Label textToServerLabel = new Label();
                //final HTML serverResponseLabel = new HTML();
                VerticalPanel dialogVPanel = new VerticalPanel();
                //dialogVPanel.addStyleName("dialogVPanel");
                dialogVPanel.add(new HTML("<b>"+e.getMessage()+"</b>"));
                dialogVPanel.setHorizontalAlignment(VerticalPanel.ALIGN_RIGHT);
                //dialogVPanel.add(closeButton);
                dialogBox.setWidget(dialogVPanel);
                //dialogBox.center();
                dialogBox.setHideOnButtonClick(true);
                dialogBox.setPredefinedButtons(Dialog.PredefinedButton.CANCEL);
                dialogBox.show();
                
                
                //Window.alert("Error: " + e.getCause().getMessage());
                //logger.log(Level.SEVERE, e.getCause().getMessage());
                //e.printStackTrace();
            }
        });*/
        
    StateManager.get().setProvider(new CookieProvider("/", null, null, GXT.isSecure()));
        Logger logger = Logger.getLogger(MainUI.class.toString());
        logger.log(Level.INFO, "starting MainUI");

        //ProductTreeView2 view2 = new ProductTreeView2();
        ProfileList profileList=new ProfileList();
        //ProductTreeView3 view2 = new ProductTreeView3();
        
        
        
        final BorderLayoutContainer con = new BorderLayoutContainer();
        con.setBorders(true);


        ContentPanel north = new ContentPanel();
        ContentPanel west = new ContentPanel();
        ContentPanel center = new ContentPanel();
        ContentPanel south = new ContentPanel();
        ContentPanel east = new ContentPanel();


        BorderLayoutData northData = new BorderLayoutData(100);
        northData.setMargins(new Margins(5));
        northData.setCollapsible(true);
        northData.setSplit(true);

        BorderLayoutData westData = new BorderLayoutData(250);
        westData.setCollapsible(true);
        westData.setSplit(true);
        westData.setCollapseMini(true);
        westData.setMargins(new Margins(0, 5, 0, 5));

        MarginData centerData = new MarginData();

        BorderLayoutData eastData = new BorderLayoutData(150);
        eastData.setMargins(new Margins(0, 5, 0, 5));
        eastData.setCollapsible(true);
        eastData.setSplit(true);

        BorderLayoutData southData = new BorderLayoutData(100);
        southData.setMargins(new Margins(5));
        southData.setCollapsible(true);
        southData.setCollapseMini(true);


        con.setNorthWidget(north, northData);
        con.setWestWidget(west, westData);
        con.setCenterWidget(profileList, centerData);
        con.setEastWidget(east, eastData);
        //con.setSouthWidget(south, southData);


        //SimpleContainer simple = new SimpleContainer();
        //simple.add(con, new MarginData(10));
        Viewport viewport = new Viewport();
        viewport.add(con);
        RootLayoutPanel.get().add(viewport);
        //RootPanel.get("startloader").removeFromParent();


        /*
         DockLayoutPanel p = uiBinder.createAndBindUi(this);
         RootLayoutPanel.get().add(p);
         */
        //getJson(1, "https://itunes.apple.com/lookup?id=909253&callback=", this);
    }

    /**
     * Cast JavaScriptObject as JsArray of StockData.
     */
    private final native iTunesData asITunesData(JavaScriptObject jso) /*-{
     return jso;
     }-*/;

    public void handleJsonResponse(JavaScriptObject obj) {
        GWT.log("received data:" + obj.toString());
        iTunesData data = asITunesData(obj);
        GWT.log("itunes data count:" + data.getCount());
        GWT.log("itunes data artist:" + data.getArtists().get(0).getName());

    }

    /**
     * Make call to remote server.
     */
    public native static void getJson(int requestId, String url, MainUI handler) /*-{
     var callback = "callback" + requestId;

     // [1] Create a script element.
     var script = document.createElement("script");
     script.setAttribute("src", url+callback);
     script.setAttribute("type", "text/javascript");

     // [2] Define the callback function on the window object.
     window[callback] = function(jsonObj) {
     // [3]
     handler.@de.mec.client.MainUI::handleJsonResponse(Lcom/google/gwt/core/client/JavaScriptObject;)(jsonObj)
     window[callback + "done"] = true;
     }

     // [4] JSON download has 1-second timeout.
     setTimeout(function() {
     if (!window[callback + "done"]) {
     handler.@de.mec.client.MainUI::handleJsonResponse(Lcom/google/gwt/core/client/JavaScriptObject;)(null);
     }

     // [5] Cleanup. Remove script and callback elements.
     document.body.removeChild(script);
     delete window[callback];
     delete window[callback + "done"];
     }, 1000);

     // [6] Attach the script element to the document body.
     document.body.appendChild(script);
     }-*/;
}

class Artist extends JavaScriptObject {                              // (1)
    // Overlay types always have protected, zero argument constructors.

    protected Artist() {
    }                                              // (2)

    public final native String getName() /*-{ return this.artistName; }-*/; // (3)
}

class iTunesData extends JavaScriptObject {                              // (1)
    // Overlay types always have protected, zero argument constructors.

    protected iTunesData() {
    }                                              // (2)

    public final native int getCount() /*-{ return this.resultCount; }-*/; // (3)

    public final native JsArray<Artist> getArtists()/*-{return this.results}-*/;
}