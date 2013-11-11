/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client;

import com.google.gwt.editor.client.Editor;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.Grid;
import com.google.gwt.user.client.ui.SimplePanel;
import com.sencha.gxt.data.shared.Converter;
import com.sencha.gxt.widget.core.client.Slider;
import com.sencha.gxt.widget.core.client.form.*;
import de.mec.client.model.Codec;
import de.mec.client.model.tools.CodecIdConverter;
import de.mec.client.model.tools.ObjectConverter;
import de.mec.client.model.tools.String2IntegerConverter;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author jan.hoelscher
 */
public abstract class PropertyItemEditor<S,T> extends Composite implements Editor<PropertyItem> {
  //public abstract void setLabel(final String l);
  
  private static final Logger LOG = Logger.getLogger(PropertyItemEditor.class.getName());
    
    //@Path("key")
    @Ignore
    protected FieldLabel  label;

    @Path("val")
    //@Ignore
    public ConverterEditorAdapter<S, T,Field<T>> converter;

    @Ignore
    //@Path("val")
    public Field<T> wert;
    
    public PropertyItemEditor(Field component, Converter c) {
        //setHeadingText("base");
        LOG.log(Level.INFO,"new PropertyItemEditor 12");
        Grid grid = new Grid(1, 2);
        setWidget(grid);
         //Label lblAge = new Label("Video Codec");
        label=new FieldLabel();
        //key.setEnabled(false);
        wert=component;//new TextField();
        if(c!=null){
          //converter=new ConverterEditorAdapter(wert, new String2IntegerConverter());
          //converter=new ConverterEditorAdapter(wert, new ObjectConverter<T>());
          converter=new ConverterEditorAdapter(wert, c);
        }else{
          converter=new ConverterEditorAdapter(wert, new ObjectConverter<T>());
        }        
        grid.setWidget(0, 0, label);
        grid.setWidget(0, 1, wert);
        
    }
    
    public void setLabel(final String l){
      label.setText(l);
    }
    
    
}
