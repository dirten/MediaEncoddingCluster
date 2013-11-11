/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.editor;

import com.google.gwt.editor.client.Editor;
import com.google.gwt.user.client.ui.Grid;
import com.sencha.gxt.widget.core.client.Slider;
import com.sencha.gxt.widget.core.client.form.FieldLabel;
import com.sencha.gxt.widget.core.client.form.TextField;
import de.mec.client.PropertyItemEditor;
import de.mec.client.model.tools.String2IntegerConverter;
import java.util.logging.Level;

/**
 *
 * @author jhoelscher
 */
public class SliderItemEditor extends PropertyItemEditor<String,Integer>{
    //private FieldLabel  label;

    //@Editor.Path("val")
    //public Slider wert;
    
    public SliderItemEditor() {
      super(new Slider(),new String2IntegerConverter());
      
      /*
        //setHeadingText("base");
        //LOG.log(Level.INFO,"new PropertyItemEditor");
        Grid grid = new Grid(1, 2);
        setWidget(grid);
         //Label lblAge = new Label("Video Codec");
        label=new FieldLabel();
        //key.setEnabled(false);
        wert=new Slider();
        
        grid.setWidget(0, 0, label);
        grid.setWidget(0, 1, wert);
        */
    }
/*
  @Override
  public void setLabel(String l) {
    label.setText(l);
  }
  */
}
