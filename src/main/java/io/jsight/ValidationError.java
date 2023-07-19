package io.jsight;

public class ValidationError {

    private String        reportedBy;
    private String        type;
    private int           code;
    private String        title;
    private String        detail;
    private ErrorPosition position;
    private String[]      trace;

    public ValidationError(
        String        reportedBy,
        String        type,
        int           code,
        String        title,
        String        detail,
        ErrorPosition position,
        String[]      trace
    ) {
        this.reportedBy = reportedBy;
        this.type       = type;
        this.code       = code;
        this.title      = title;
        this.detail     = detail;
        this.position   = position;
        this.trace      = trace;             
    }


    public String getReportedBy() {
        return this.reportedBy;
    }
    public String getType(){
        return this.type;
    }
    public int    getCode(){
        return this.code;
    }
    public String getTitle(){
        return this.title;
    }
    public String getDetail(){
        return this.detail;
    }
    public ErrorPosition getPosition(){
        return this.position;
    }
    public String[] getTrace(){
        return this.trace;
    }

    public String toJSON() {
        return JSight.SerializeError("json", this);
    }
}