/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client;

import com.google.gwt.editor.client.Editor;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.Grid;
import com.google.gwt.user.client.ui.SimplePanel;
import com.sencha.gxt.widget.core.client.form.TextField;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author jan.hoelscher
 */
public class PropertyItemEditor extends SimplePanel implements Editor<PropertyItem> {
    private static final Logger LOG = Logger.getLogger(PropertyItemEditor.class.getName());
    
    @Path("key")
    public TextField  name;
    
    @Path("val")
    public TextField wert;
    
    public PropertyItemEditor() {
        //setHeadingText("base");
        LOG.log(Level.INFO,"new PropertyItemEditor");
        Grid grid = new Grid(1, 2);
        setWidget(grid);
         //Label lblAge = new Label("Video Codec");
        name=new TextField();
        //key.setEnabled(false);
        wert=new TextField();
        
        grid.setWidget(0, 0, name);
        grid.setWidget(0, 1, wert);
        
    }
    
}
