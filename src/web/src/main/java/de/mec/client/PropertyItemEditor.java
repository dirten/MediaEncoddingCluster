/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client;

import com.google.gwt.editor.client.Editor;
import com.google.gwt.event.logical.shared.ValueChangeEvent;
import com.google.gwt.event.logical.shared.ValueChangeHandler;
import com.google.gwt.event.shared.HandlerRegistration;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.Grid;
import com.sencha.gxt.data.shared.Converter;
import com.sencha.gxt.widget.core.client.form.*;
import de.mec.client.model.tools.ObjectConverter;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author jan.hoelscher
 */
public abstract class PropertyItemEditor extends Composite implements Editor<PropertyItem> {
    //public abstract void setLabel(final String l);

    private static final Logger LOG = Logger.getLogger(PropertyItemEditor.class.getName());
    private FieldLabel label;
    private String _key;

    public String getKey() {
        return _component.getName();
    }

    public void setKey(String key) {
        _component.setName(key);
    }
    private Field _component;
    @Path("val")
    public ConverterEditorAdapter<String, Object, Field<Object>> converter;

    public PropertyItemEditor(Field component, Converter c) {
        _component = component;
        
        //setHeadingText("base");
        //LOG.log(Level.INFO,"new PropertyItemEditor 12");
        Grid grid = new Grid(1, 2);
        initWidget(grid);
        //Label lblAge = new Label("Video Codec");
        label = new FieldLabel();
        //key.setEnabled(false);
        //wert=component;//new TextField();
        if (c != null) {
            //converter=new ConverterEditorAdapter(wert, new String2IntegerConverter());
            //converter=new ConverterEditorAdapter(wert, new ObjectConverter<T>());
            converter = new ConverterEditorAdapter(component, c);
        } else {
            converter = new ConverterEditorAdapter(component, new ObjectConverter<String>());
        }
        grid.setWidget(0, 0, label);
        grid.setWidget(0, 1, component);

    }

    public void setLabel(final String l) {
        label.setText(l);
    }

    public HandlerRegistration addValueChangeHandler(ValueChangeHandler<Field> handler) {
        return _component.addHandler(handler, ValueChangeEvent.getType());
    }
}
