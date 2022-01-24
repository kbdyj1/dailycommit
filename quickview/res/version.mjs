import version from "version";
import { name } from "info";

export function getVersion() {
    if (lang == "ko") {
        return "큐트" + version;
    } else {

        return "Qt" + version;
    }
}

export function getName() {
    return name;
}
