// The contents of this file are subject to the Mozilla Public License
// Version 1.1 (the "License"); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://www.mozilla.org/MPL/
// 
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
// License for the specific language governing rights and limitations
// under the License.
// 
// The Original Code is ______________________________________.
// 
// The Initial Developer of the Original Code is ________________________.
// Portions created by ______________________ are Copyright (C) ______
// _______________________. All Rights Reserved.
// 
// Contributor(s): ______________________________________.
// 
// Alternatively, the contents of this file may be used under the terms
// of the _____ license (the  "[___] License"), in which case the
// provisions of [______] License are applicable instead of those
// above.  If you wish to allow use of your version of this file only
// under the terms of the [____] License and not to allow others to use
// your version of this file under the MPL, indicate your decision by
// deleting  the provisions above and replace  them with the notice and
// other provisions required by the [___] License.  If you do not delete
// the provisions above, a recipient may use your version of this file
// under either the MPL or the [___] License."

#include "record.h"

bool Record::operator == (const Record & x)const
{
    if (fromX != x.fromX || fromY != x.fromY
            ||toX != x.toX || toY != x.toY
    ) return false;
    if (ifEat != x.ifEat || ifJiangjun != x.ifJiangjun) return false;
    if (ifEat)
    {
        if (dstColor != x.dstColor || dstType != x.dstType)
            return false;
    }
    if (x.fenStr != fenStr) return false;
    return true;
}

QString Record::ToMoveString()
{
    QString s;
    s.append('a' + fromY);
    s.append('0' + fromX);
    s.append('a' + toY);
    s.append('0' + toX);
    return s;
}

uint qHash(const Record& key)
{
    int p = (1 << 25) + 7;
    int ans = 1;
    for (auto & i : key.fenStr)
        ans = ((long long)(ans + ans * i.toLatin1()) << 1) % p;
    ans = ((long long)(ans + ans * key.fromX) << 1) % p;
    ans = ((long long)(ans + ans * key.fromY) << 1) % p;
    ans = ((long long)(ans + ans * key.toX) << 1) % p;
    ans = ((long long)(ans + ans * key.toY) << 1) % p;
    ans = ((long long)(ans + ans * key.ifEat) << 1) % p;
    ans = ((long long)(ans + ans * key.ifJiangjun) << 1) % p;
    return ans;
}
