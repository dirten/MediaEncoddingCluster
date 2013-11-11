/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.editor;

import com.google.gwt.editor.client.Editor;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.Grid;
import com.google.gwt.user.client.ui.SimplePanel;
import com.sencha.gxt.widget.core.client.Slider;
import com.sencha.gxt.widget.core.client.form.FieldLabel;
import com.sencha.gxt.widget.core.client.form.TextField;
import com.sencha.gxt.widget.core.client.form.DateTimePropertyEditor;
import de.mec.client.PropertyItemEditor;
import de.mec.client.model.tools.ObjectConverter;

/**
 *
 * @author jhoelscher
 */
public class TextItemEditor extends PropertyItemEditor<String,String>{
      //private FieldLabel  label;

  //@Editor.Path("val")
  //  public TextField wert;
    
    public TextItemEditor() {
      super(new TextField(), new ObjectConverter<String>());
      /*
        //setHeadingText("base");
        //LOG.log(Level.INFO,"new PropertyItemEditor");
        Grid grid = new Grid(1, 2);
        setWidget(grid);
         //Label lblAge = new Label("Video Codec");
        label=new FieldLabel();
        //key.setEnabled(false);
        wert=new TextField();
        
        grid.setWidget(0, 0, label);
        grid.setWidget(0, 1, wert);
        */
    }
/*
  public void setLabel(String l) {  
    label.setText(l);
  }
*/
}
