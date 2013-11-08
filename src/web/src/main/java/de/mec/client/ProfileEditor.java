/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client;

import com.google.gwt.editor.client.Editor;
import com.google.gwt.user.client.ui.Grid;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.ScrollPanel;
import com.google.gwt.user.client.ui.SimplePanel;
import com.google.gwt.user.client.ui.TextBox;
import com.sencha.gxt.widget.core.client.ContentPanel;
import com.sencha.gxt.widget.core.client.TabItemConfig;
import com.sencha.gxt.widget.core.client.TabPanel;
import com.sencha.gxt.widget.core.client.container.NorthSouthContainer;
import com.sencha.gxt.widget.core.client.container.SimpleContainer;
import com.sencha.gxt.widget.core.client.form.TextField;
import de.mec.client.model.Profile;

/**
 *
 * @author jan.hoelscher
 */
public class ProfileEditor extends SimpleContainer implements Editor<Profile> {

    public TextField name;
    public TextField uuid;
    public AudioEditor audio;
    public FormatEditor format;

    @Ignore
    public VideoEditor video;
    //public VideoPropertyListEditor video;

    //@Ignore
    @Editor.Path("videoMap")
    public VideoPropertyListEditor videoListEditor;

    public ProfileEditor() {
        //setHeadingText("Edit Profile!");
        //setPredefinedButtons(PredefinedButton.YES, PredefinedButton.NO, PredefinedButton.CANCEL);
        //setHideOnButtonClick(true);
        setResize(true);

        NorthSouthContainer c = new NorthSouthContainer();
        
        setWidget(c);
        
        Grid grid = new Grid(3, 3);
        c.setNorthWidget(grid);
        grid.setSize("100%", "100");

        Label lblNewLabel = new Label("name");
        grid.setWidget(0, 0, lblNewLabel);

        name = new TextField();
        name.setWidth(350);
        grid.setWidget(0, 1, name);

        Label lblAge = new Label("uuid");
        grid.setWidget(1, 0, lblAge);

        uuid = new TextField();
        uuid.setWidth(350);
        grid.setWidget(1, 1, uuid);

        audio = new AudioEditor();
        video=new VideoEditor();
        format=new FormatEditor();
        videoListEditor=new VideoPropertyListEditor();

        TabPanel panel = new TabPanel();
        c.setSouthWidget(panel);
        
        panel.setBorders(true);
        //panel.setSize("100%", "100%");
        //panel.setTabScroll(true);
        //panel.setAnimScroll(true);
        //scroller.setSize("100%", "100");
        ContentPanel pa=new ContentPanel();
        
        //pa.setSize("100%", "100%");
        pa.setResize(true);

        panel.setResizeTabs(true);
        
        panel.add(format, new TabItemConfig("Format", false));
        panel.add(videoListEditor, new TabItemConfig("Video", false));
        panel.add(audio, new TabItemConfig("Audio", false));
        //c.forceLayout();
        //grid.setWidget(2, 1, panel);
    }
    /*
     @Ignore
     public TextButton getHideButton() {
     return super.getHideButton();
     }*/
}
